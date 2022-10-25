#include "PolicyEngine.h"
#include "Policy.h"
#include "Handle.h"
#include <shared_mutex>
#include <thread>
#include <log.h>
#include "patch.h"
#include "TalkWithCC.h"
#include "log.h"
#include "PolicyModelList.h"
#include <unistd.h>


PolicyEngine *PolicyEngine::_ins = nullptr;
bool PolicyEngine::_running_flag = false;

std::shared_timed_mutex __mutex;

PolicyEngine *PolicyEngine::Ins() {
    if (_ins == nullptr) _ins = new PolicyEngine;
    return _ins;
}

PolicyEngineReturn PolicyEngine::Init(const std::string& cchost, const std::string& ccport, const std::string& ccuser, const std::string& ccpwd, const std::string& model,
                        unsigned int sync_interval_seconds) {
    if (Ins()->_running_flag) return POLICY_ENGINE_SUCCESS;
    Ins()->_cchost = cchost;
    Ins()->_ccport = ccport;
    Ins()->_ccuser = ccuser;
    Ins()->_ccpwd = ccpwd;
    //Ins()->_tag = tag;
    Ins()->_model_name = model;
    if (sync_interval_seconds < 60) 
        Ins()->_sync_interval_seconds = 60;
    else 
        Ins()->_sync_interval_seconds = sync_interval_seconds;
    Ins()->_is_actions_updated = false;
    Ins()->_policy_hash = 0;
    //if (POLICY_ENGINE_SUCCESS != Ins()->Update()) return POLICY_ENGINE_CCCONNECT_ERROR;

    //get policy save file
    std::string strDataFolder = CommonFun::GetProgramDataFolder();
    if (strDataFolder.empty()){
        Log::WriteLog(log_error, "PolicyEngine::Init, ProgramData folder is empty" );
    }

    Ins()->_strPolicySaveFile = strDataFolder + "policy"; 
    Ins()->_strPolicyModelSaveFile = strDataFolder + "policy_models"; 
    Log::WriteLog(log_info, "PolicyEngine::Init, policy save file:%s", Ins()->_strPolicySaveFile.c_str() ); 
    

   // Ins()->Update();
    Ins()->_running_flag = true;
    std::thread thread(PolicyEngine::Sync);
    thread.detach();
    return POLICY_ENGINE_SUCCESS;
}

bool PolicyEngine::GetTablePolicyinfo(TablePolicyInfoMap & tbs_map, S4HException &exc){
    if(_is_actions_updated){
        //remove old
        tbs_map.clear();
        //copy new
        {
            std::shared_lock<std::shared_timed_mutex> readerLock(__mutex);
            tbs_map.insert(_tbs_map.begin(),_tbs_map.end());
        }
        _is_actions_updated = false;
    }
    return true;
}

PolicyEngineReturn PolicyEngine::Exit() {
    if (!_running_flag) return POLICY_ENGINE_MODULE_NOT_INIT;
    Ins()->_running_flag = false;
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngine::~PolicyEngine() {

}

void PolicyEngine::DownloadPolicyToFile(int fd)
{
   std::vector<Json::Value> jsons;
    PolicyModelList pmodels;
    bool r = PolicyHelper::DownloadPolicys(_cchost, _ccport, _ccuser, _ccpwd, _model_name, jsons, pmodels);
    if (!r){
         Log::WriteLog(log_error, "PolicyEngine::DownloadPolicyToFile return for download policy failed.");
        return ;
    }

    //get target policy model
    PolicyModel model;
    bool bfind = pmodels.AddPmByName(_model_name,model);
    if(!bfind){
        Log::WriteLog(log_error, "PolicyEngine::DownloadPolicyToFile not find policy model by model name in configuration");
         //free talk
        delete(pmodels.GetTalk());
        pmodels.SetTalk(NULL);
        return ;
    }

    Log::WriteLog(log_info, "PolicyEngine::DownloadPolicyToFile: json size:%u", jsons.size() );

    //save policy models
    SavePolicyModelsToFile(pmodels);
        
    //clear file when we download policy success.
    ftruncate(fd, 0);
    fsync(fd);

    MaskConditionMap map;
    auto itJson = jsons.begin();
    while (itJson != jsons.end()){
        Json::Value& jsonValue = *itJson;

        //get obligation
        Json::Value Obligations;
//        if(!jsonValue["data"].isNull()){
//            if(!jsonValue["data"]["allowObligations"].isNull()){
//                Obligations.append( jsonValue["data"]["allowObligations"]);
//            }
//            if(!jsonValue["data"]["denyObligations"].isNull()){
//                Obligations.append(jsonValue["data"]["denyObligations"]);
//            }
//        }
        if(!jsonValue["allowObligations"].isNull()){
            Obligations.append(  jsonValue["allowObligations"]);
        }
        if(!jsonValue["denyObligations"].isNull()){
            Obligations.append(  jsonValue["denyObligations"]);
        }
        if(!Obligations.isArray()){
            continue;
        }

        bool bfind = false;
        for (unsigned int i = 0; i < Obligations.size(); ++i)
        {

            Json::Value &json_obg = Obligations[i];
            for (unsigned int j = 0;j < json_obg.size(); ++j)
            {
                Json::Value & sub = json_obg[j];
                int modelid = sub["policyModelId"].asInt();
                if (model._id == modelid)
                {
                    bfind = true;
                    break;
                }
            }
            if(bfind){
                break;
            }
        }
        if(bfind){
            //contains obligation what we want, //save policy to file
            Json::StreamWriterBuilder builder;
            builder.settings_["indentation"] = "";
            std::string strJString = Json::writeString(builder, jsonValue);
            strJString += "\n";
            ssize_t nWrite = write(fd, strJString.c_str(), strJString.size());
            if(nWrite!=strJString.size()){
                Log::WriteLog(log_error, "%s: write to file failed. nWrite:%u,  strJString.size:%u, errno:%d", __func__, nWrite, strJString.size(), errno );
            }
            
        }

        itJson++;
    }

    fsync(fd);

    //free
    delete(pmodels.GetTalk());
    pmodels.SetTalk(NULL);
}

void PolicyEngine::SavePolicyModelsToFile(PolicyModelList& pmodels)
{
     //get all needed policy model
    PolicyModel model;
    bool bfind = pmodels.AddPmByName(_model_name,model);
    if(!bfind){
        Log::WriteLog(log_error, "PolicyEngine::SavePolicyModelsToFile not find policy model by model name in configuration");
    }

    {
        PolicyModel model;
        bool bfind = pmodels.AddPmByName("host",model);
        if(!bfind){
            Log::WriteLog(log_error, "PolicyEngine::SavePolicyModelsToFile not find policy model host ");
        }
    }

    {
        PolicyModel model;
        bool bfind = pmodels.AddPmByName("user",model);
        if(!bfind){
            Log::WriteLog(log_error, "PolicyEngine::SavePolicyModelsToFile not find policy model user ");
        }
    }

    {
        PolicyModel model;
        bool bfind = pmodels.AddPmByName("application",model);
        if(!bfind){
            Log::WriteLog(log_error, "PolicyEngine::SavePolicyModelsToFile not find policy model application ");
        }
    }

    //save
    FILE* pFilePM = fopen(_strPolicyModelSaveFile.c_str(), "w");
    if (NULL==pFilePM){
        Log::WriteLog(log_error, "PolicyEngine::SavePolicyModelsToFile open save file failed. file:%s, errno:%d", _strPolicyModelSaveFile.c_str(), errno);
        return;
    }


    const std::vector<PolicyModel>& allPolicyModel = pmodels.GetAllPolicyModels();
    for (auto itPM = allPolicyModel.begin(); itPM!=allPolicyModel.end(); itPM++){

        //write JSON
        fwrite(itPM->_strJson.c_str(), itPM->_strJson.length(), 1, pFilePM);
        fwrite("\n", 1, 1, pFilePM);

        //write PreAttributeJSON
        fwrite(itPM->_strPreAttributeJson.c_str(), itPM->_strPreAttributeJson.length(), 1, pFilePM);
        fwrite("\n", 1, 1, pFilePM);
    }


    //close
    fflush(pFilePM);
    fclose(pFilePM);
    pFilePM = NULL;
}

void PolicyEngine::ReadPolicyModelsFromFile(PolicyModelList& pmList)
{
    //open file
    FILE* pFilePM = fopen(_strPolicyModelSaveFile.c_str(), "r");
    if (NULL==pFilePM){
        Log::WriteLog(log_error, "PolicyEngine::ReadPolicyModelsFromFile open save file failed. file:%s, errno:%d", _strPolicyModelSaveFile.c_str(), errno);
        return;
    }

    //read line by line
    const int nMaxBufSize = 1 * 1024 * 1024; //we asssume the max size of a policy model is 1M 
    char* pLinePolicyModel= new char[nMaxBufSize];
    char* pLinePreAttribute= new char[nMaxBufSize];
    while (!feof(pFilePM)){

        char* pDataPolicyModel = fgets(pLinePolicyModel, nMaxBufSize-1, pFilePM);//A terminating null character is automatically appended after the characters copied to pLine.
        if (pDataPolicyModel)  {
           
            //read pre-attribute line
            char* pDataPreAttribute = NULL;
            if (!feof(pFilePM)) {
               pDataPreAttribute = fgets(pLinePreAttribute, nMaxBufSize-1, pFilePM);
            }
             
            //construct policy model object
            PolicyModel pm;
            bool bValidJson = pm.ParseFromJson(pDataPolicyModel);
            if (bValidJson){
                if(pDataPreAttribute && (pm._type==PolicyModel::PM_SUB_USER ||
                                         pm._type==PolicyModel::PM_SUB_HOST || 
                                         pm._type==PolicyModel::PM_SUB_APP)){
                   Log::WriteLog(log_info, "PolicyEngine::ReadPolicyModelsFromFile, added pre-attribute to policy model, name:%s", pm._name.c_str() );
                   pm.AddPreAttribute(pDataPreAttribute);
                }
                
                //added to list
                Log::WriteLog(log_info, "PolicyEngine::ReadPolicyModelsFromFile, added Policy model to list, name:%s", pm._name.c_str() );
                pmList.AddPMStatic(pm);
            }     
        }
        
    }
    
    //free
    delete[] pLinePolicyModel; pLinePolicyModel=NULL;
    delete[] pLinePreAttribute; pLinePreAttribute=NULL;

    fclose(pFilePM);
    pFilePM = NULL;
}

void PolicyEngine::ReadPolicyFromFile(int fd)
{
    //read policy models first
    PolicyModelList pmList;
    ReadPolicyModelsFromFile(pmList);


    //seek to the begining of the file
    lseek(fd, 0, SEEK_SET);

    //get file size
    struct stat fileStat;
    fstat(fd, &fileStat);
    Log::WriteLog(log_info, "PolicyEngine::ReadPolicyFromFile: file size:%u", fileStat.st_size);

    off_t bufSize = std::min(fileStat.st_size, (long)10*1024*1024); //one policy is about 5k. so limit the file size to 10M is enough.

    if (bufSize>0){
        std::unique_ptr<char> pContent(new char[bufSize+1]);
        ssize_t readCount =  read(fd, pContent.get(), bufSize);
        Log::WriteLog(log_info, "PolicyEngine::ReadPolicyFromFile: read file count:%u", readCount);
        if (readCount>=0){
            pContent.get()[readCount]=0;
        }

        {//check hash value
            std::hash<std::string> hash_str;
            size_t hash_val = hash_str(std::string(pContent.get()));
            if(_policy_hash == hash_val){
                pContent = NULL;
                return;
            } else {
                _policy_hash = hash_val;
            }
        }

        char* pData = pContent.get();
        bool bHaveNewLine = true;
        TablePolicyInfoMap tbs_map ;
        while (bHaveNewLine) {
            //get the line data
            std::string strJson;
            char* pNewLine = strchr(pData, '\n');
            if (pNewLine){
                pNewLine[0]=0;

                strJson = pData;

                //next line
                pData = pNewLine;
                pData++;
            }
            else{
                bHaveNewLine = false;
                strJson = pData;
            }

            if (strJson.empty()) {
                continue; //may be a emtpy line
            }
            

            //construct json
            Json::CharReaderBuilder builder;
            std::unique_ptr<Json::CharReader> reader( builder.newCharReader() );
            Json::Value jsonValue;
            std::string strErrors;
            bool parsingSuccessful = reader->parse(strJson.c_str(), strJson.c_str() + strJson.size(), &jsonValue, &strErrors);
            if (!parsingSuccessful) {
                Log::WriteLog(log_info, "PolicyEngine::ReadPolicyFromFile: parse json failed.");
                continue;
            }
            
            Policy policy;
            if (policy.ParseFromJson(jsonValue, &pmList)==POLICY_ENGINE_SUCCESS)
            {
                S4HException ex;
//                policy.GetMaskInfos(map, ex);
//                policy.GetResTables(tables,ex);
//                policy.GetActions(actions,ex);
//                for(auto it:tables){
//                    if(dup_map.end()==dup_map.find(it)){
//                        DenyUpdateItemMap * dupi_map = new DenyUpdateItemMap;
//                        dup_map.insert(std::make_pair(it, dupi_map));
//                        policy.GetUpdateControl(*dupi_map, ex);
//                    }
//                }
                policy.GetPolicyBasicInfo(tbs_map,ex);

            } 
             
        }
        
        pContent = NULL;

        {
            std::lock_guard<std::shared_timed_mutex> writerLock(__mutex);
            _tbs_map.swap(tbs_map);
            _is_actions_updated = true;
        }

        //printf
        char szLog[1024];
        std::string strLog = "AllTablePolicyInfo:";
        for (auto itTable = _tbs_map.begin(); itTable != _tbs_map.end(); itTable++)
        {
            TablePolicyInfo &refPolicyInfo = itTable->second;
            sprintf_s(szLog, 1024, "table:%s, actions:%d,refPolicyInfo._bfilter=%d, refPolicyInfo._bmask=%d, refPolicyInfo._bdup=%d\t\t",
                      itTable->first.c_str(), refPolicyInfo._actions, refPolicyInfo._bfilter, refPolicyInfo._bmask, refPolicyInfo._bdup);
            strLog += szLog;
        }
        Log::WriteLog(log_info, strLog.c_str());

    }
    
}



PolicyEngineReturn PolicyEngine::Update() {
  //  Log::WriteLog(log_trace, "PolicyEngine::Update enter.");

    //open policy file
    int fd = open(_strPolicySaveFile.c_str() , O_RDWR|O_CREAT|O_CLOEXEC ,S_IRUSR|S_IWUSR);
    if (0==fd){
         Log::WriteLog(log_error, "PolicyEngine::Update open policy save file failed, errno:%d", errno);
    }
    
    //lock file. if the file is locked by other process. this call will be blocked. and
    //if the process terminate without call unlock, the file will automatic unlocked when process exit.
    int nLock= lockf(fd, F_LOCK, 0 );
    if (nLock==0){
        
        //check if we need to download policy
        struct stat fileStat={0};
        fstat(fd, &fileStat);

        if (fileStat.st_size<=0 || difftime(time(0), fileStat.st_mtim.tv_sec)>_sync_interval_seconds){
            Log::WriteLog(log_info, "PolicyEngine::Update DownloadPolicyToFile");
            DownloadPolicyToFile(fd);
        }

        //read policy from file
        ReadPolicyFromFile(fd);
        
        //unlock, actually this is unnessnary, because calling close() will unlock the file
       lockf( fd, F_ULOCK, 0 );
    }
    else{
         Log::WriteLog(log_error, "PolicyEngine::Update lock file failed. errno:%d", errno);
    }

    //close file
    close(fd);  
    fd=0;
    
    return POLICY_ENGINE_SUCCESS;
}

void PolicyEngine::Sync() {
    
    while (_running_flag ) {
        Ins()->Update();
        std::this_thread::sleep_for(std::chrono::seconds(Ins()->_sync_interval_seconds+5/*+5s to guarantee the policy cache file be updated next time*/));
    }
    delete (_ins);
    _ins = nullptr;
}


void PolicyEngine::UpdateSyncInterval(unsigned int iseconds){
    _sync_interval_seconds = iseconds;
}

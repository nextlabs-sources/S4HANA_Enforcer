#ifndef CLASS_POLICY_ENGINE_H
#define CLASS_POLICY_ENGINE_H

#include <vector>
#include <string>
#include "StructDefine.h"
#include "Handle.h"

class Policy;
class PolicyModelList;

class PolicyEngine {
public:
    static PolicyEngine *Ins();
    static PolicyEngineReturn Init(const std::string& cchost, const std::string& ccport, const std::string& ccuser, const std::string& ccpwd, const std::string& tag,
                            unsigned int sync_interval_seconds);
    static PolicyEngineReturn Exit();
    void UpdateSyncInterval(unsigned int iseconds);
    bool GetTablePolicyinfo(TablePolicyInfoMap & tbs_map, S4HException &exc);
    ~PolicyEngine();
private:
    PolicyEngineReturn Update();
    static void Sync();

#ifdef _WIN32
    void DownloadPolicyToFile(HANDLE fd);
    void ReadPolicyFromFile(HANDLE fd);
#else
    void DownloadPolicyToFile(int fd);
    void ReadPolicyFromFile(int fd);
#endif

    void SavePolicyModelsToFile(PolicyModelList& pmList);
    void ReadPolicyModelsFromFile(PolicyModelList& pmList);

private:
    std::string _cchost;
    std::string _ccport;
    std::string _ccuser;
    std::string _ccpwd;
    std::string _tag;
    std::string _model_name;
    unsigned int _sync_interval_seconds;

    std::string _strPolicySaveFile;
    std::string _strPolicyModelSaveFile;

    //std::vector<Policy*> _policys;
    bool _is_actions_updated;
    TablePolicyInfoMap _tbs_map;

    static PolicyEngine *_ins;
    static bool _running_flag;

    size_t  _policy_hash ; // to update parser json from string

    #ifdef _WIN32
    HANDLE _MutexPolicyFile;
    #endif


};


#endif
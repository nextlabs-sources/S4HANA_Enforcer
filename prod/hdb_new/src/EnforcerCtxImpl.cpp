#include "EnforcerCtxImpl.h"
#include "talk_with_hdb.h"
#include "string.h"
#include "tool.h"
#include <mutex>
#include <log.h>

BindParameters::BindParameters(const SQLDBC_UInt4 Index, const SQLDBC_HostType Type, void *paramAddr,
        SQLDBC_Length *LengthIndicator, const SQLDBC_Length Size, const SQLDBC_Bool Terminate) :
        _Index(Index), _Type(Type), _paramAddr(paramAddr), _LengthIndicator(LengthIndicator), _Size(Size), _Terminate(Terminate) {
  // didn't allocate new memory when record parameters, because for at this state, the memory may didn't contains the actually value
  _bBindWithNewValue = false;
}

BindParameters::~BindParameters() {

    if (_bBindWithNewValue){
        delete[] (unsigned char*)_paramAddr;
        _paramAddr = NULL;
    }
    

}

//unsigned long long EnforcerCtxImpl::_uSizeAll = 0;

EnforcerCtxImpl::EnforcerCtxImpl(void *original_stmt, std::shared_ptr<EMS4HAst> pAst) :
    _original_stmt(original_stmt),
    _enforced_stmt(nullptr),
    _past(pAst) {
    _bEnforced = false; 
    _uCalledAPI = API_NONE;
    }

EnforcerCtxImpl::~EnforcerCtxImpl()
{
    //release enforcer statement
	if (_enforced_stmt != nullptr) {
		SQLDBC::SQLDBC_Connection* pConn = SQLDBC_Statement_getConnection_Old(_enforced_stmt);
		if (pConn) {
			SQLDBC_Connection_ReleaseStatement_Prepared_Old(pConn, _enforced_stmt);
			_enforced_stmt = nullptr;
		}
	}

    //free bind parameters
    auto itPara = _bind_parameters.begin();
    while (itPara != _bind_parameters.end())
    {
        delete itPara->second; 
       itPara++;
    }
    _bind_parameters.clear();

    //free replaced parameter buf
    FreeReplacedParameterBuf();
    
}

void LogEnforcerCtxInfo()
{
    static time_t s_tLast= time(0);
    if (difftime(time(0), s_tLast) > 60 * 5 )
     {
         theLog->WriteLog(log_info, "DAE logged enforced context count:%llu", BINDINGS.size());
         s_tLast =  time(0);
    }
}

void EnforcerCtxImpl::RecordParameter(const SQLDBC_UInt4     Index,
                     const SQLDBC_HostType  Type,
                     void                  *paramAddr,
                     SQLDBC_Length         *LengthIndicator,
                     const SQLDBC_Length    Size,
                     const SQLDBC_Bool      Terminate) {
    auto fd = _bind_parameters.find(Index);
    if (fd == _bind_parameters.end()) {
        _bind_parameters.insert(std::make_pair(Index, new BindParameters(Index, Type, paramAddr, LengthIndicator, Size, Terminate)));
    } else {
        delete (fd->second);
        fd->second = new BindParameters(Index, Type, paramAddr, LengthIndicator, Size, Terminate);
    }
}

bool EnforcerCtxImpl::GetParamValueByPosition(uint iRow, uint position, std::string& strValue)
{
    auto fd = _bind_parameters.find(position);
    if (fd != _bind_parameters.end()) {
        BindParameters* bindParam = fd->second;
        if (bindParam->_Type == SQLDBC_HOSTTYPE_UCS2_SWAPPED)
        {
            unsigned char* pAddr = (unsigned char*)bindParam->_paramAddr;
            void* pData = pAddr + iRow * bindParam->_Size;
            std::u16string sql_param((char16_t *)pData, bindParam->_Size/sizeof(char16_t));
            strValue = CommonFun::ToUTF8(sql_param);
            strValue.erase(strValue.find_last_not_of(" ") + 1);  
            return true;
        }
    }
   return false;
}

BindParameters* EnforcerCtxImpl::GetBindParamByPosition(uint position) const
{
    auto fd = _bind_parameters.find(position);
    if (fd != _bind_parameters.end()) {
       return fd->second;
    }
   return NULL;

}

bool EnforcerCtxImpl::OriginStmtBindParameters(SQLDBC::SQLDBC_PreparedStatement* pStmt, S4HException& e)
{
    if (pStmt == nullptr) {
        e.SetError(S4H_BIND_PARAM_ERROR, "pStmt null");
        return false;
    }
    SQLDBC_Retcode rc = SQLDBC_OK;
    for (auto it : _bind_parameters) {
        rc = bind_parameter_old(pStmt, it.second->_Index, it.second->_Type, it.second->_paramAddr, it.second->_LengthIndicator, it.second->_Size, it.second->_Terminate);
        if (rc != SQLDBC_OK) {
            e._code = S4H_BIND_PARAM_ERROR;
            theLog->WriteLog(log_fatal, "OriginStmtBindParameters return false:%d", rc);
            CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pStmt)), e._detail);
            return false;
        }
    }

    e._code = S4H_SUCCESS;
    return true;
}

bool EnforcerCtxImpl::EnforcedStmtBindParameters(S4HException& e) {
    if (_enforced_stmt == nullptr) {
        e.SetError(S4H_BIND_PARAM_ERROR, "_enforced_stmt null");
        return false;
    }
    SQLDBC_Retcode rc = SQLDBC_OK;
    for (auto it : _bind_parameters) {
        rc = bind_parameter_old(_enforced_stmt, it.second->_Index, it.second->_Type, it.second->_paramAddr, it.second->_LengthIndicator, it.second->_Size, it.second->_Terminate);
        if (rc != SQLDBC_OK) {
            e._code = S4H_BIND_PARAM_ERROR;
            theLog->WriteLog(log_fatal, "EnforcedStmtBindParameters return false:%d", rc);
            CommonFun::FromUTF8(SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(_enforced_stmt)), e._detail);
            return false;
        }
    }

    //setbatchsize
    if(_unBatchSize!=0){
        SQLDBC_PreparedStatement_setBatchSize_Old(_enforced_stmt, _unBatchSize);
    }

    e._code = S4H_SUCCESS;
    return true;
}

void EnforcerCtxImpl::AllocateOriginalValueBuf(const std::u16string& strOriginalValue, void** ppBuf,long long* /*SQLDBC_Length*/pLen){
    *pLen = strOriginalValue.length()*2;

    //find exist one
    char* pNewBuf = NULL;
    auto itParam= _replacedParametersBuf.begin();
    while(itParam != _replacedParametersBuf.end() ){
        if (memcmp(*itParam, strOriginalValue.c_str(), *pLen)==0) {
            pNewBuf = *itParam;
            break;
        }

        itParam++;
    }

    //create new one if not exist
    if (NULL==pNewBuf) {
        pNewBuf = new char[*pLen];
        memcpy(pNewBuf, strOriginalValue.c_str(), *pLen);
        AddReplacedParameterBuf(pNewBuf);
    }
    
    *ppBuf = pNewBuf;  
}


void EnforcerCtxImpl::AddReplacedParameterBuf(char* pBuf)
{ 
    _replacedParametersBuf.push_back(pBuf); 
}

void EnforcerCtxImpl::FreeReplacedParameterBuf()
{
     std::list<char*>::iterator itBuf = _replacedParametersBuf.begin();
     while (itBuf!=_replacedParametersBuf.end()){
         delete[] (*itBuf);
         itBuf++;
     }
    _replacedParametersBuf.clear();
}

bool EnforcerCtxImpl::ReplaceParamValueByPosition(uint iRow, uint position, const std::string& strParamValue)
{
    BindParameters* pBindItem = GetBindParamByPosition(position);
    if(NULL==pBindItem){
        return false;
    }

    if (pBindItem->_Type == SQLDBC_HOSTTYPE_UCS2_SWAPPED)
    {
        const SQLDBC_Length nOrigDataSize = pBindItem->_Size;
        SQLDBC_Length* pLenInd = pBindItem->_LengthIndicator;

        //get data point and size
        unsigned char* pAddr = (unsigned char*)pBindItem->_paramAddr;
        void* pData = pAddr + iRow * nOrigDataSize;

        //empty the full buffer
        std::u16string emptyData(nOrigDataSize/sizeof(char16_t), u' ');
        memcpy(pData, emptyData.c_str(), nOrigDataSize );

        //replace the new value
        size_t  copyLen = strParamValue.size()*sizeof(char16_t)<nOrigDataSize ? (strParamValue.size()*sizeof(char16_t)) : nOrigDataSize;    //strParamValue never over flow
        std::u16string u16OriginalValue = CommonFun::FromUTF8(strParamValue);
        memcpy(pData, u16OriginalValue.c_str(), copyLen );

        //set actually length
        //pBindItem->_Size = copyLen;
        //*(pBindItem->_LengthIndicator) = copyLen;

        theLog->WriteLog(log_info, "EnforcerCtxImpl::ReplaceParamValue, iRow:%d, position:%d, value:%s, nOrigDataSize:%d, copyLen:%d, LenInd:%d", 
                                                                           iRow, position, strParamValue.c_str(), nOrigDataSize, copyLen, *pLenInd);
    }

    return true;
}

std::map<void*, EnforcerCtxImpl*> BINDINGS;
std::mutex mtx;

EnforcerCtxImpl *register_enforcer_ctx(void *original_stmt, std::shared_ptr<EMS4HAst> pAst) {
    EnforcerCtxImpl *ret = nullptr;
    mtx.lock();
    auto fd = BINDINGS.find(original_stmt);
    if (fd == BINDINGS.end()) {
        ret = new EnforcerCtxImpl(original_stmt, pAst);
        BINDINGS.insert(std::make_pair(original_stmt, ret));
    } else {
        ret = fd->second;
        if(ret) {
            ret->SetAst(pAst);
        }
    }
    mtx.unlock();
    return ret;
}

EnforcerCtxImpl *get_enforcer_ctx(void *original_stmt) {
    EnforcerCtxImpl *ret = nullptr;
    mtx.lock();
    auto fd = BINDINGS.find(original_stmt);
    if (fd != BINDINGS.end()) ret = fd->second;
    mtx.unlock();
    return ret;
}

void remove_enforcer_ctx(void *original_stmt){
    mtx.lock();
    auto fd = BINDINGS.find(original_stmt);
    if (fd != BINDINGS.end()) {
          delete fd->second;
          BINDINGS.erase(original_stmt);
    }
       
    mtx.unlock();
}


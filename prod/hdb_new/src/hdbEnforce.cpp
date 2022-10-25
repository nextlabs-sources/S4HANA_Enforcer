#include "hdbEnforce.h"
#include "log.h"
#include "tool.h"
#include "talk_with_hdb.h"
#include "NxlDbDataMgr.h"

void HdbEnforce::ReplaceKeyMastValueWithOriginalValue(SQLDBC::SQLDBC_PreparedStatement* pPreparedStmt, EnforcerCtxImpl* pCtx)
{
    bool bNeedRebind = false;
    {
        auto itPara = pCtx->_bind_parameters.begin();
        while (itPara != pCtx->_bind_parameters.end())
        {
            BindParameters* pBindParam = itPara->second; 
            if (pBindParam && pBindParam->_paramAddr && 
                (pBindParam->_Type==SQLDBC_HOSTTYPE_UCS2_SWAPPED) && (pBindParam->_Size<=256 && pBindParam->_Size>0) )
            {
                //get param value
                std::u16string sql_param((char16_t*)pBindParam->_paramAddr, pBindParam->_Size/2) ;
                std::string utf8Value = CommonFun::ToUTF8(sql_param);
                utf8Value.erase(utf8Value.find_last_not_of(" ") + 1); //trim space 

                NxlDBDataMgr* pNxlDataMgr = NxlDBDataMgr::Instance();
                if (pNxlDataMgr->IsKeyMaskData(utf8Value))
                {
                    std::string strOriginalValue = pNxlDataMgr->GetOriginalValuByMaskValue(SQLDBC_Statement_getConnection_Old(pPreparedStmt), utf8Value);
                    theLog->WriteLog(log_info, "ReplaceKeyMastValueWithOriginalValue GetOriginalValuByMaskValue maskvalue:%s, original:%s", utf8Value.c_str(), strOriginalValue.c_str() );
                    if (!strOriginalValue.empty())
                    {
                        std::u16string u16OriginalValue = CommonFun::FromUTF8(strOriginalValue);
                        void* pNewParamAddr=NULL;
                        SQLDBC_Length NewSize = pBindParam->_Size;
                        pCtx->AllocateOriginalValueBuf(u16OriginalValue, &pNewParamAddr, (SQLDBC_Length*)(&NewSize));
                        if (pNewParamAddr)
                        {
                            //here we can't just call bind_parameter_old to re-bind this parameter. 
                            //we need to clear all parameter and re-bind all parameter
                            theLog->WriteLog(log_info, "ReplaceKeyMastValueWithOriginalValue replace new parameter");  
                            pBindParam->_paramAddr = pNewParamAddr;
                            pBindParam->_Size = NewSize;
                            bNeedRebind = true;
                        }
                            
                    }   
                }    
            
            }
            
            itPara++;
        }
    }

    if (bNeedRebind)
    {
        SQLDBC_Retcode ret = SQLDBC_PreparedStatement_clearParameters_Old(pPreparedStmt);
        if (ret!=SQLDBC_OK) {
            theLog->WriteLog(log_fatal, "ReplaceKeyMastValueWithOriginalValue clearParameters failed. ret=%d, msg=%s", ret, SQLDBC_ErrorHndl_getErrorText_Old( &SQLDBC_ConnectionItem_error_Old(pPreparedStmt)) );
        }
        else
        {
             theLog->WriteLog(log_info, "ReplaceKeyMastValueWithOriginalValue re-bind parameter");
            auto itPara = pCtx->_bind_parameters.begin();
            while (itPara != pCtx->_bind_parameters.end())
            {
                BindParameters* pBindParam = itPara->second; 
                bind_parameter_old(pPreparedStmt, pBindParam->_Index, pBindParam->_Type, pBindParam->_paramAddr,
                        pBindParam->_LengthIndicator, pBindParam->_Size, pBindParam->_Terminate);

                itPara++;
            }
        }
    }
    
}

void HdbEnforce::CollectParamToAST(EnforcerCtxImpl* pCtx, EMS4HAst* ast)
{ 
    auto itPara = pCtx->_bind_parameters.begin();
    while (itPara != pCtx->_bind_parameters.end())
    {
        BindParameters *pBindParam = itPara->second;
        if (pBindParam->_paramAddr == NULL)
        {
            continue;
        }

        SQLDBC_HostType Type = pBindParam->_Type;
        if ((Type == SQLDBC_HOSTTYPE_UCS2_SWAPPED) && (pBindParam->_Size <= 512 && pBindParam->_Size > 0))
        {
            std::u16string sql_param((char16_t *)pBindParam->_paramAddr, pBindParam->_Size / 2);
            std::string utf8Value = CommonFun::ToUTF8(sql_param);
            ast->CollectParam(pBindParam->_Index, utf8Value);
        }
        else if (Type == SQLDBC_HOSTTYPE_INT1 || Type == SQLDBC_HOSTTYPE_INT2 || Type == SQLDBC_HOSTTYPE_INT4 || Type == SQLDBC_HOSTTYPE_INT8)
        {
            int64_t data = *(int64_t *)(pBindParam->_paramAddr);
            ast->CollectParam(pBindParam->_Index, data);
        }
        else if (Type == SQLDBC_HOSTTYPE_UINT1 || Type == SQLDBC_HOSTTYPE_UINT2 || Type == SQLDBC_HOSTTYPE_UINT4 || Type == SQLDBC_HOSTTYPE_UINT8)
        {
            uint64_t data = *(uint64_t *)(pBindParam->_paramAddr);
            ast->CollectParam(pBindParam->_Index, data);
        }
        else if (Type == SQLDBC_HOSTTYPE_ODBCDATE || Type == SQLDBC_HOSTTYPE_FLOAT)
        {
            /* code */
        }
        else if (Type == SQLDBC_HOSTTYPE_DECIMAL /*BCD encoded number*/)
        {
            uint64_t uValue = CommonFun::ConvertBCDNumber((unsigned char*)pBindParam->_paramAddr, 5);
            ast->CollectParam(pBindParam->_Index, uValue);
            //unsigned char* pData = (unsigned char*)pBindParam->_paramAddr;
            //theLog->WriteLog(log_info, "ConvertBCD index=%d, value=%u, data=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",  pBindParam->_Index, uValue, pData[0],pData[1],pData[2],pData[3],pData[4],pData[5],pData[6] );         
        }

        itPara++;
    }

}
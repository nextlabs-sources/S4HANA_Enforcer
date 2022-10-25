#include <stdio.h>
#include <string.h>
#include <string>
#include <thread>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif 

#pragma warning(push)
#pragma warning(disable: 4702)
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#pragma warning(pop)

#include "log.h"
#include "NXLHttpClientAsync.h"
#include "NXS4HConfig.h"
#include "tool.h"
#include "AESEncrypt.h"
#include <vector>

std::string g_strCCPWD;
std::string g_strJPCPWD;
std::string g_strTokenPath;
std::string g_strPDPPath;


#define JPC_V_202205  202205

std::string DecryptPWD(const std::string& strCipher, const char* szWhereInfo)
{
    std::string strPlainText;
    try {
        if (std::string::npos == strCipher.find_first_not_of("0123456789ABCDEF")) {
            CommonFun::AesEncryptor theAes;
            strPlainText = theAes.DecryptString(strCipher);
        }
        else {
            LOG::WriteLog("An unencrypted password string ('%s') was detected in %s", strCipher.c_str(), szWhereInfo);
        }
    }
    catch (const std::exception &e)  {
        LOG::WriteLog("exception happend on Decrpt, what:%s", e.what() );
    }

    return strPlainText;
}


bool ValidJPCServer(unsigned int msTimeout)
{
    STEPS::SetStepStatus(STEP_VALID_JPC_SERVER, false);

    const std::string& strJPCHost = CONF_JPCHOST;
    const std::string& strJPCPort = CONF_JPCPORT;
    const std::string& strPDPPath = g_strPDPPath;

    LOG::WriteLog("\nBegin %s with:%s:%s%s", STEP_VALID_JPC_SERVER,  strJPCHost.c_str(), strJPCPort.c_str(), strPDPPath.c_str() );

	//set request paramter
	http::request<http::string_body> req{ http::verb::get, strPDPPath, 11 };

	req.set(http::field::user_agent, "QueryCloudAZ_BoostBeast");
	req.set(http::field::connection, "close");

   //send query
	boost::beast::flat_buffer buffer;
	http::response<http::string_body> res;
	try
	{
        boost::shared_ptr<NXLHttpClientAsync> clt(new NXLHttpClientAsync(strJPCHost.c_str(), strJPCPort.c_str(), msTimeout));
        clt->SetBuffer(&req, &buffer, &res);
        QueryStatus status = clt->Run();
        if (status != QueryStatus::QS_S_OK)  {
            LOG::WriteLog("NXLHttpClientAsync.run failed!, status=%d, strStatus=%s", status, g_QueryStatusStr[status].c_str());
            LOG::WriteLog("%s failed.", STEP_VALID_JPC_SERVER);
            return false;
        }
        else if(res.result()!=http::status::ok){
            LOG::WriteLog("%s failed. http status:%d", STEP_VALID_JPC_SERVER, res.result() );
            return false;
        }
        else{
            STEPS::SetStepStatus(STEP_VALID_JPC_SERVER, true);
            LOG::WriteLog("%s success.", STEP_VALID_JPC_SERVER);
             return true;
        }

		return true;
	}
	catch (const std::exception& e)
	{
		LOG::WriteLog("Exception on %s. msg:%s", __func__, e.what() );
		return false;
	}
    LOG::WriteLog("%s failed.", STEP_VALID_JPC_SERVER);
    return false;
}
 
bool ValidCCUser(unsigned int msTimeout)
{
    STEPS::SetStepStatus(STEP_VALID_CC_USER, false);

    const std::string& strCCUser= CONF_CCUSER;
    const std::string& strCCPWD=g_strCCPWD;
    const std::string& strCCHost =  CONF_CCHOST;
    const std::string& strCCPort =  CONF_CCPORT;
    const std::string& strTokenPath = []()->std::string{
            std::string tokenPath = "/cas/token";  //to valid with CC, we need cc token path
            const std::string &cfgTokenPath = g_conf->GetJPCTokenPath();
            if (!cfgTokenPath.empty())  {
                if (cfgTokenPath[0] == '/') {
                    tokenPath = cfgTokenPath;
                }
                else {
                    tokenPath = "/";
                    tokenPath += cfgTokenPath;
                }
            }
            return tokenPath;
    }();

    LOG::WriteLog("\nBegin Valid CC user with:%s:%s%s", strCCHost.c_str(), strCCPort.c_str(), strTokenPath.c_str() );

    //
    std::string strUrlPwd = CommonFun::UrlEncode(strCCPWD);

    //format body
	const static std::string AuthBodyFormat = "grant_type=client_credentials&client_id=%s&client_secret=%s&expires_in=%d";
	char AuthBody[1024] = { 0 };
    snprintf(AuthBody, sizeof(AuthBody), AuthBodyFormat.c_str(), strCCUser.c_str(), strUrlPwd.c_str(), 1800);


	//set request paramter
	http::request<http::string_body> req{ http::verb::post, strTokenPath, 11, AuthBody };

	req.set(http::field::user_agent, "QueryCloudAZ_BoostBeast");
	req.set(http::field::connection, "keep-alive");
	req.set(http::field::content_type, "application/x-www-form-urlencoded");
	req.set(http::field::content_length, strlen(AuthBody));
	req.set("Pragma", "no-cache");

   //send query
	boost::beast::flat_buffer buffer;
	http::response<http::string_body> res;
	try
	{
        boost::shared_ptr<NXLHttpClientAsync> clt(new NXLHttpClientAsync(strCCHost.c_str(), strCCPort.c_str(), msTimeout));
        clt->SetBuffer(&req, &buffer, &res);
        QueryStatus status = clt->Run();
        if (status != QueryStatus::QS_S_OK)  {
            LOG::WriteLog("NXLHttpClientAsync.run failed!, status=%d, strStatus=%s", status, g_QueryStatusStr[status].c_str());
            LOG::WriteLog("%s failed.", STEP_VALID_CC_USER);
            return false;
        }

        if (res.result() == http::status::ok)
		{
          //  LOG::WriteLog("ValidCCUser for %s. response:%s", szWhereInfo, res.body().c_str() );

			std::stringstream ss(res.body());
			boost::property_tree::ptree ptResponse;

			boost::property_tree::read_json<boost::property_tree::ptree>(ss, ptResponse);
	
			//check error
			boost::optional<boost::property_tree::ptree&> ptError = ptResponse.get_child_optional("error");
			if (ptError) {

                std::string strErrorInfo = ptResponse.get<std::string>("error");
                if (CommonFun::StrCaseCmp(strErrorInfo, "unauthorized_client")==0){
                    LOG::WriteLog("%s success.", STEP_VALID_CC_USER);
                    STEPS::SetStepStatus(STEP_VALID_CC_USER, true);
                    return true;
                }
                else{
                     LOG::WriteLog("%s. response:%s", __func__, res.body().c_str() );
                     LOG::WriteLog("%s failed.", STEP_VALID_CC_USER);
				    return false;
                } 
			}
            else{
                LOG::WriteLog("%s success.", STEP_VALID_CC_USER);
                STEPS::SetStepStatus(STEP_VALID_CC_USER, true);
                return true;
            }
		}
        else{
            LOG::WriteLog("%s failed. http status:%d", STEP_VALID_CC_USER, res.result() );
            STEPS::SetStepStatus(STEP_VALID_CC_USER, false);
            return false;
        }
	}
	catch (const std::exception& e)
	{
		LOG::WriteLog("Exception on ValidUser for %s. msg:%s", __func__, e.what() );
	}
     LOG::WriteLog("%s failed.", STEP_VALID_CC_USER);
    return false;
}

bool ValidJPCUser(bool bJPC202205, unsigned int msTimeout)
{
    STEPS::SetStepStatus(STEP_VALID_JPC_USER, false);

    const std::string& strJPCUser= CONF_JPCUSER;
    const std::string& strJPCPWD=g_strJPCPWD;
    const std::string& strTokenHost =  bJPC202205 ? CONF_JPCHOST : CONF_CCHOST ;
    const std::string& strTokenPort =  bJPC202205 ? CONF_JPCPORT : CONF_CCPORT ;
    const std::string& strTokenPath =  g_strTokenPath;
   
    LOG::WriteLog("\nBegin Valid JPC user with:%s:%s%s", strTokenHost.c_str(), strTokenPort.c_str(), strTokenPath.c_str() );
    //
    const std::string strUrlPwd = CommonFun::UrlEncode(strJPCPWD);

    //format body
	const static std::string AuthBodyFormat = "grant_type=client_credentials&client_id=%s&client_secret=%s&expires_in=%d";
	char AuthBody[1024] = { 0 };
    snprintf(AuthBody, sizeof(AuthBody), AuthBodyFormat.c_str(), strJPCUser.c_str(), strUrlPwd.c_str(), 1800);


	//set request paramter
	http::request<http::string_body> req{ http::verb::post, strTokenPath, 11, AuthBody };

	req.set(http::field::user_agent, "QueryCloudAZ_BoostBeast");
	req.set(http::field::connection, "keep-alive");
	req.set(http::field::content_type, "application/x-www-form-urlencoded");
	req.set(http::field::content_length, strlen(AuthBody));
	req.set("Pragma", "no-cache");

   //send query
	boost::beast::flat_buffer buffer;
	http::response<http::string_body> res;
	try
	{
        boost::shared_ptr<NXLHttpClientAsync> clt(new NXLHttpClientAsync(strTokenHost.c_str(), strTokenPort.c_str(), msTimeout));
        clt->SetBuffer(&req, &buffer, &res);
        QueryStatus status = clt->Run();
        if(status != QueryStatus::QS_S_OK){
            LOG::WriteLog("NXLHttpClientAsync.run failed!, status=%d, strStatus=%s", status, g_QueryStatusStr[status].c_str() );
            LOG::WriteLog("%s failed.", STEP_VALID_JPC_USER);
            return  false;
        }

		if (res.result() == http::status::ok)
		{
          //  LOG::WriteLog("ValidCCUser for %s. response:%s", szWhereInfo, res.body().c_str() );

			std::stringstream ss(res.body());
			boost::property_tree::ptree ptResponse;

			boost::property_tree::read_json<boost::property_tree::ptree>(ss, ptResponse);
	
			//check error
			boost::optional<boost::property_tree::ptree&> ptError = ptResponse.get_child_optional("error");
			if (ptError) {
                LOG::WriteLog("ValidUser for %s. response:%s", __func__, res.body().c_str() );
                LOG::WriteLog("%s failed.", STEP_VALID_JPC_USER);
				return false;
    
			}
            else{
                LOG::WriteLog("%s success.", STEP_VALID_JPC_USER);
                STEPS::SetStepStatus(STEP_VALID_JPC_USER, true);
                return true;
            }
		}else{
            LOG::WriteLog("%s failed. http status:%d", STEP_VALID_JPC_USER, res.result() );
            STEPS::SetStepStatus(STEP_VALID_JPC_USER, false);
            return false;
        }
	}
	catch (const std::exception& e)
	{
		LOG::WriteLog("Exception on %s. msg:%s", __func__, e.what() );
	}
    LOG::WriteLog("%s failed.", STEP_VALID_JPC_USER);
    return false;
}

void VerifyConfig(const char* szCfg)
{
    STEPS::SetStepStatus(STEP_READ_CONFIG, false);

    const std::string strConfFile = szCfg; 
    //read config
    g_conf = new NXS4HConfig();
    bool bread = g_conf->InitConfig(strConfFile);
    LOG::WriteLog("Read config %s", bread ? "Success" : "Failed");
    if(bread) {
        STEPS::SetStepStatus(STEP_READ_CONFIG, true);
    }else{
        STEPS::SetStepStatus(STEP_READ_CONFIG, false);
        return;
    }
     

    //decrypt ccpwd, jpcpwd
    if(!CONF_CCPWD.empty()){
        g_strCCPWD = DecryptPWD(CONF_CCPWD, "CCPWD");
        LOG::WriteLog("Decrpt CCPWD:%s", g_strCCPWD.empty() ? "Failed" : "Success");
    }
   

    g_strJPCPWD = DecryptPWD(CONF_JPCPWD, "JPCPwd");
    LOG::WriteLog("Decrpt JPCPWD:%s", g_strJPCPWD.empty() ? "Failed" : "Success");

    //check if need config cc information
    const int ccVersion=g_conf->GetJPCVersion();
    const bool bVer202205=ccVersion>=JPC_V_202205;
    const bool bUseTableList = g_conf->UseSecurityTableList();
    const bool bNeedCCInfo = (!bVer202205) || (!bUseTableList);
     LOG::WriteLog("IsCC202205:%s, UseTableList:%s", bVer202205 ? "true" : "false", bUseTableList ? "true" : "false");

    if(bNeedCCInfo){
        if(CONF_CCUSER.empty() || g_strCCPWD.empty() || CONF_CCHOST.empty() || CONF_CCPORT.empty() ){
             LOG::WriteLog("have not configure cc information. please check cchost, ccport, ccuser, ccpwd");
             STEPS::SetStepStatus(STEP_READ_CONFIG, false);
             return;
        }
    }
    
    //get token path
    g_strTokenPath= bVer202205 ? "/dpc/oauth" : "/cas/token";
    const std::string& cfgTokenPath=g_conf->GetJPCTokenPath();
	if (!cfgTokenPath.empty()) {
		if (cfgTokenPath[0]=='/') {
			g_strTokenPath = cfgTokenPath;
		}
		else {
			g_strTokenPath = "/";
			g_strTokenPath += cfgTokenPath;
		}
	}

    //get pdp path
    g_strPDPPath="/dpc/authorization/pdp";
    const std::string& cfgPDPPath=g_conf->GetJPCPDPPath();
	if(!cfgPDPPath.empty()){
		if (cfgPDPPath[0]=='/') {
			g_strPDPPath = cfgPDPPath;
		}
		else {
			g_strPDPPath = "/";
			g_strPDPPath += cfgPDPPath;
		}
	}

    //
    unsigned int msTimeout = g_conf->GetQueryPCTimeout() * 1000;

    //valid jpc user with CC/JPC
    ValidJPCUser(bVer202205, msTimeout);


    //valid CC user with CC
    if(bNeedCCInfo){
        ValidCCUser(msTimeout);
    }
    
    //if not 202205, check JPC status,because we didn't connect to JPC at all
    if(!bVer202205){
        ValidJPCServer(msTimeout);
    }

}
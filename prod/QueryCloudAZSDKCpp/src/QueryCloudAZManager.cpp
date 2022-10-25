
//#include <NXLHttpClientAsync.h>
#include <NXLHttpClientAsync.h>
#include "QueryCloudAZManager.h"
#include "PolicyRequest.h"
#include "Attributes.h"
//#include <atlbase.h>
#include "CommonFun.h"
#include "NXLHttpClient.h"
#include <string.h>

QueryCloudAZMgr::QueryCloudAZMgr()
{
	m_WriteLogFun=nullptr;
}

bool QueryCloudAZMgr::Init(const char* wszPCHost, const char* wszPcPort,
	                       const char* wszOAuthServiceHost, const char* wszOAuthPort,
	                       const char* strClientId, const char* wszClientSecret,
	                        const char* wszTokenPath, const char* wszPDPPath,  unsigned int timeout,
						   QueryJPCLogFun logFun, bool bVer202205)
{
	m_strPCHost = wszPCHost;
	m_strPCPort = wszPcPort;
	m_strOAuthHost = wszOAuthServiceHost;
	m_strOAuthPort = wszOAuthPort;
	m_strClientID = strClientId;
	m_strClientSecret = wszClientSecret;
	m_strClientSecret = UrlEncode(m_strClientSecret);
	m_WriteLogFun = logFun;
	m_connPool.SetConnectCount(1);
	UpdateTimeout(timeout);

	m_strTokenPath= bVer202205 ? "/dpc/oauth" : "/cas/token";
	if (wszTokenPath && strlen(wszTokenPath)>1) {
		if (wszTokenPath[0]=='/') {
			m_strTokenPath = wszTokenPath;
		}
		else {
			m_strTokenPath = "/";
			m_strTokenPath += wszTokenPath;
		}
	}

    m_strPDPPath="/dpc/authorization/pdp";
	if(wszPDPPath && strlen(wszPDPPath)>1){
		if (wszPDPPath[0]=='/') {
			m_strPDPPath = wszPDPPath;
		}
		else {
			m_strPDPPath = "/";
			m_strPDPPath += wszPDPPath;
		}
	}
    
	//log
	char szLog[512];
	snprintf(szLog, sizeof(szLog), "token path:%s,pdp path:%s", m_strTokenPath.c_str(), m_strPDPPath.c_str() );
	WriteLog(log_info, szLog);

	return true; //RequestToken();
}

bool QueryCloudAZMgr::UpdateTimeout(unsigned int nTimeout)
{
	m_timeout = (nTimeout<1 ? 1 : nTimeout)*1000;
    m_connPool.SetTimeout(m_timeout);
	return true;
}

QueryStatus QueryCloudAZMgr::CheckSingleResource(const IPolicyRequest* ceRequest, IPolicyResult** pcResult)
{
	//assign a connection
	QueryStatus qs = QS_E_InternalError;
	CloudAZConnection* pConn = m_connPool.AssignConnection();
	if (pConn)
	{
		int nTryTime = 0;
		do
		{
			qs = pConn->CheckSingleResource(ceRequest, pcResult);
			if (ErrCheck(pConn, qs))
			{
				break;
			}
			nTryTime++;
		} while (nTryTime < MaxTryTimes);
	}

	return qs;
}

QueryStatus QueryCloudAZMgr::CheckMultiResource(const IPolicyRequest** pcRequest, int nRequestCount, IPolicyResult** pcResult)
{
	//check limit
	if (nRequestCount>MAX_REQUEST_IN_MULTICHECK)
	{
		return QS_E_TooManyRequest;
	}

	//assign a connection
	QueryStatus qs = QS_E_InternalError;
	CloudAZConnection* pConn = m_connPool.AssignConnection();
	if (pConn)
	{
		int nTryTime = 0;
		do
		{
			qs = pConn->CheckMultiResource(pcRequest, nRequestCount, pcResult);
			if (ErrCheck(pConn, qs))
			{
				break;
			}
			nTryTime++;
		} while (nTryTime < MaxTryTimes);
	}

	return qs;
}

IPolicyRequest* QueryCloudAZMgr::CreatePolicyRequest()
{
	return dynamic_cast<IPolicyRequest*>(new PolicyRequest());
}

IAttributes* QueryCloudAZMgr::CreateCEAttr()
{
	return dynamic_cast<IAttributes*>(new Attributes());
}

void QueryCloudAZMgr::FreeCEAttr(IAttributes* pAttr)
{
	delete pAttr;
}

void QueryCloudAZMgr::FreePolicyRequest(IPolicyRequest* pRequest)
{
	delete pRequest;
}

void QueryCloudAZMgr::FreePolicyResult(IPolicyResult* pResult)
{
	delete pResult;
}

bool QueryCloudAZMgr::RequestToken()
{
	//format body
	const static std::string AuthBodyFormat = "grant_type=client_credentials&client_id=%s&client_secret=%s&expires_in=%d";
	char AuthBody[1024] = { 0 };
	//sprintf_s(AuthBody, 1024, AuthBodyFormat.c_str(), GetClientID().c_str(), GetClientSecret().c_str(), 1800);
    snprintf(AuthBody, sizeof(AuthBody), AuthBodyFormat.c_str(), GetClientID().c_str(), GetClientSecret().c_str(), 1800);

#if 1


	//set request paramter
	http::request<http::string_body> req{ http::verb::post, GetTokenPath().c_str(), 11, AuthBody };

	req.set(http::field::user_agent, "QueryCloudAZ_BoostBeast");
	req.set(http::field::connection, "close");
	req.set(http::field::content_type, "application/x-www-form-urlencoded");
	req.set(http::field::content_length, strlen(AuthBody));
//	req.set("Service", "EVAL");
	//req.set("Version", "1.0");
	req.set("Pragma", "no-cache");

   //send query
	boost::beast::flat_buffer buffer;
	http::response<http::string_body> res;
	try
	{
        //NXLHttpClient httpClient(GetOAuthHost().c_str(), GetOAuthPort().c_str());
		//httpClient.Request(req, buffer, res);
        boost::shared_ptr<NXLHttpClientAsync> clt(new NXLHttpClientAsync(GetOAuthHost().c_str(), GetOAuthPort().c_str(),10000));
        clt->SetBuffer(&req, &buffer, &res);
        QueryStatus status = clt->Run();
        if(status != QueryStatus::QS_S_OK){
            const std::string& strLog="NXLHttpClientAsync.run failed!, status=" + std::to_string(status);
            QueryCloudAZMgr::Instance()->WriteLog(log_error, strLog.c_str());
            return  false;
        }

		if (res.result() == http::status::ok )
		{
			std::stringstream ss(res.body());
			boost::property_tree::ptree ptResponse;

			boost::property_tree::read_json<boost::property_tree::ptree>(ss, ptResponse);
	
			//check error
			boost::optional<pt::ptree&> ptError = ptResponse.get_child_optional("error");
			if (!ptError)
			{
				std::string strToken = ptResponse.get<std::string>("access_token");
				std::string strTokenType = ptResponse.get<std::string>("token_type");
				int nExpire = ptResponse.get<int>("expires_in");

				//set token with write lock
				{
					boost::unique_lock<boost::shared_mutex> lock(m_mutexToken);	// write lock
					m_Token = strToken;
					m_TokenType = strTokenType;
					m_nTokenExpier = nExpire;
				}

				return (!m_Token.empty()) && (!m_TokenType.empty()) && (m_nTokenExpier > 0);
			}
		}
	}
	catch (const std::exception& e)
	{
		std::string err = "";
		err += "Query Policy httpClient.Request exception";
		err += e.what();
		WriteLog(log_error, err.c_str());
		return false;
	}



#else
	USES_CONVERSION;

	std::string strHost = GetOAuthHost() + ":" + GetOAuthPort();
	web::http::client::http_client httpClient(A2W(strHost.c_str()));
	try
	{
		//set request paramter
		web::http::http_request request(web::http::methods::POST);
		request.set_request_uri(web::http::uri(L"/cas/token"));
		request.headers().add(L"Connection",  "Close");
		request.headers().add(L"User-Agent","QueryCloudAZcpp");
		request.set_body(AuthBody, "application/x-www-form-urlencoded");
		
		web::http::http_response ServerInforResponse = httpClient.request(request).get();
		if (ServerInforResponse.status_code() == web::http::status_codes::OK)
		{
			web::json::value jsonValue = ServerInforResponse.extract_json().get();

			//check error
			if (!jsonValue.has_field(L"error"))
			{
				std::wstring wstrToken = COMMONFUN::GetStringFromJsonValue(jsonValue, L"access_token");
				std::wstring wstrTokenType = COMMONFUN::GetStringFromJsonValue(jsonValue, L"token_type");
				int nExpire = COMMONFUN::GetIntFromJsonValue(jsonValue, L"expires_in");

				//set token with write lock
				{
					boost::unique_lock<boost::shared_mutex> lock(m_mutexToken);	// write lock
					m_Token = W2A(wstrToken.c_str());
					m_TokenType = W2A(wstrTokenType.c_str());
					m_nTokenExpier = nExpire;
				}

				return (!m_Token.empty()) && (!m_TokenType.empty()) && (m_nTokenExpier>0);
			}
		}
	}
	catch (...)
	{
	}
#endif 

	
	return false;
}

bool QueryCloudAZMgr::ErrCheck(CloudAZConnection* pConn, QueryStatus qs)
{
	if (qs == QS_E_DisConnect)
	{//reset and try again
		WriteLog(log_info, "qs == QS_E_DisConnect, reset and try again");
		pConn->Reset();
		return false;
	}
	else if (qs == QS_E_Unauthorized)
	{//need refresh token and try again
		WriteLog(log_info, "qs == QS_E_Unauthorized, refresh token and try again");
		RequestToken();
		return false;
	}
    else if (qs == QS_E_Timeout)
    {//need refresh token and try again
        WriteLog(log_info, "qs == QS_E_Timeout, try again");
       // RequestToken();
        return false;
    }
    else if (qs == QS_E_ServiceUnavailable)
    {//need refresh token and try again
        WriteLog(log_info, "qs == QS_E_Unauthorized, try again");
        //RequestToken();
        return false;
    }
	else
	{
		return true;
	}
}

//
// Created by jeff on 2022/7/1.
//

#ifndef PROD_DAEHTTPCLIENT_H
#define PROD_DAEHTTPCLIENT_H

#include <boost/asio/io_service.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

// The query status from PC. 
enum QueryStatus
{
	QS_S_OK,
	QS_E_Failed,
	QS_E_Unauthorized,
	QS_E_BadRequest,
	QS_E_ServiceUnavailable,
	QS_E_MissAttributes,
	QS_E_InternalError,
	QS_E_TooManyRequest,
	QS_E_DisConnect,
	QS_E_Timeout,
	QS_E_EmptyToken,
    QS_E_ResolveFailed,
    QS_E_WriteFailed,
    QS_E_ReadFailed,
};

static std::map<QueryStatus,std::string> g_QueryStatusStr={ {QueryStatus::QS_S_OK, "S_OK"},
                                                   {QueryStatus::QS_E_Failed, "E_Failed"},
                                                   {QueryStatus::QS_E_Unauthorized, "E_Unauthorized"},
                                                   {QueryStatus::QS_E_BadRequest, "E_BadRequest"},
                                                    {QueryStatus::QS_E_ServiceUnavailable, "E_ServiceUnavailable"},
                                                    {QueryStatus::QS_E_MissAttributes, "E_MissAttributes"},
                                                    {QueryStatus::QS_E_InternalError, "E_InternalError"},
                                                    {QueryStatus::QS_E_TooManyRequest, "E_TooManyRequest"},
                                                    {QueryStatus::QS_E_DisConnect, "E_DisConnect" },
                                                    {QueryStatus::QS_E_Timeout, "E_Timeout"} ,
                                                    {QueryStatus::QS_E_EmptyToken, "E_EmptyToken"},
                                                    {QueryStatus::QS_E_ResolveFailed, "E_ResolveFailed" },
                                                    {QueryStatus::QS_E_WriteFailed, "E_WriteFailed" },
                                                    {QueryStatus::QS_E_ReadFailed, "E_ReadFailed" }
                                                };


using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http; // from <boost/beast/http.hpp>

class NXLHttpClientAsync : public boost::enable_shared_from_this<NXLHttpClientAsync>{

public:
    NXLHttpClientAsync(const char* szServer, const char* szPort, unsigned int msTimeout);
    virtual ~NXLHttpClientAsync(){ Clear();}

    void SetBuffer(http::request<http::string_body> *req , boost::beast::flat_buffer *flat_buffer ,    http::response<http::string_body> *res);
    QueryStatus Run();

    QueryStatus RunSync();

private:
    void OnResolve(const boost::system::error_code& ec, tcp::resolver::results_type results);
    void OnConnect(const boost::system::error_code& ec);
    void OnWrite(const boost::system::error_code&  ec);
    void OnRead(const boost::system::error_code&  ec);
    void OnTimeout(const boost::system::error_code&  ec);
    void Clear();

private:
    http::request<http::string_body> *_req = NULL;
    boost::beast::flat_buffer * _flat_buffer = NULL;
    http::response<http::string_body> *_res = NULL;
private:
    std::string m_strServer;
    std::string m_strPort;
    std::string m_strHost;
    unsigned int m_msTimeout = 2000;//default timeout 3000 ms

    //about socket
    boost::asio::io_service m_ioService;

    tcp::socket*  m_pHttpSocket;

    ssl::context m_SSLCtx;
    ssl::stream<tcp::socket>* m_pHttpsStream;

    bool m_https = false;


    bool m_resolveFailed = false;
    bool m_connectFailed = false;
    bool m_writeFailed = false;
    bool m_readFailed = false;
    bool m_useBreak = false;
    bool m_useBreak_timeout = false;
};


#endif //PROD_DAEHTTPCLIENT_H

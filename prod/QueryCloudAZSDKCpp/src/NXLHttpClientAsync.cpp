//
// Created by jeff on 2022/7/1.
//

#include "NXLHttpClientAsync.h"
#include <boost/bind.hpp>

NXLHttpClientAsync::NXLHttpClientAsync(const char* szServer, const char* szPort, unsigned int msTimeout) :
m_SSLCtx{ ssl::context::sslv23_client }
{
    m_strServer = szServer;
    std::transform(m_strServer.begin(), m_strServer.end(), m_strServer.begin(), tolower);
    m_strPort = szPort;
    m_pHttpsStream = NULL;
    m_pHttpSocket = NULL;
    m_https = (0==m_strServer.find("https://")) ;
    //get host
    m_strHost = m_strServer.substr(m_https ? strlen("https://") : strlen("http://"));
    m_msTimeout = msTimeout;
}

void NXLHttpClientAsync::SetBuffer(http::request<http::string_body> *req , boost::beast::flat_buffer *flat_buffer, http::response<http::string_body> *res){
    _req = req;
    _flat_buffer =  flat_buffer;
    _res = res;
}

void NXLHttpClientAsync::Clear()
{
    if (m_pHttpsStream)
    {
        m_pHttpsStream->next_layer().cancel();
        m_pHttpsStream->next_layer().close();
        delete m_pHttpsStream;
        m_pHttpsStream = NULL;
    }

    if (m_pHttpSocket)
    {
        m_pHttpSocket->cancel();
        m_pHttpSocket->close();
        delete m_pHttpSocket;
        m_pHttpSocket = NULL;
    }
}

void NXLHttpClientAsync::OnConnect(const boost::system::error_code& ec){
    //printf("testx --OnConnect--, ec:%d\n", ec);
    if(ec){
       // printf("testx --OnConnect--%s\n",ec.message().c_str());
        m_connectFailed = true;
        m_useBreak = true;
        return;
    }
    if(m_https){
        m_pHttpsStream->handshake(ssl::stream_base::client);
        //
        http::async_write((*m_pHttpsStream), *_req, boost::bind(&NXLHttpClientAsync::OnWrite, shared_from_this(),boost::asio::placeholders::error));
    }else {
        //
        http::async_write((*m_pHttpSocket), *_req, boost::bind(&NXLHttpClientAsync::OnWrite, shared_from_this(),boost::asio::placeholders::error));
    }
}

void NXLHttpClientAsync::OnRead(const boost::system::error_code&  ec){
    //printf("testx --OnRead--:%d\n", ec);
     if(ec){
       // printf("testx --OnWrite--%s\n",ec.message().c_str());
        m_readFailed = true;
     }
    m_useBreak = true;
}

void NXLHttpClientAsync::OnWrite(const boost::system::error_code& ec){
   // printf("testx --OnWrite--:%d\n", ec);
    if(ec){
        //printf("testx --OnWrite--%s\n",ec.message().c_str());
        m_writeFailed = true;
        m_useBreak = true;
        return;
    }
    //boost::ignore_unused(n);
    if(m_https){
        http::async_read(*m_pHttpsStream, *_flat_buffer, *_res, boost::bind(&NXLHttpClientAsync::OnRead, shared_from_this(),boost::asio::placeholders::error));
    } else {
        http::async_read(*m_pHttpSocket,  *_flat_buffer, *_res, boost::bind(&NXLHttpClientAsync::OnRead, shared_from_this(),boost::asio::placeholders::error));
    }
}

void NXLHttpClientAsync::OnResolve(
        const boost::system::error_code &ec,
        tcp::resolver::results_type results)
{
    //printf("--OnResolve--\n");
    if(ec){
        //printf("--OnResolve--%s\n",ec.message().c_str());
        m_resolveFailed = true;
        m_useBreak = true;
        return;
    }

    if(m_https){
        m_pHttpsStream = new ssl::stream<tcp::socket>{ m_ioService, m_SSLCtx};
        boost::asio::async_connect(m_pHttpsStream->next_layer(), results.begin(), results.end(),
                boost::bind(&NXLHttpClientAsync::OnConnect, shared_from_this(),boost::asio::placeholders::error));//
    } else {
        m_pHttpSocket = new tcp::socket(m_ioService);
        boost::asio::async_connect(*m_pHttpSocket, results.begin(), results.end(),
                boost::bind(&NXLHttpClientAsync::OnConnect, shared_from_this(),boost::asio::placeholders::error));
    }

}

void NXLHttpClientAsync::OnTimeout(const boost::system::error_code&  ec){
    m_useBreak_timeout = true;
    //printf("------OnTimeout-----------\n");
}

void on_timer_static(const boost::system::error_code&  ec, int * falg){
    if (ec != boost::asio::error::operation_aborted){
        (*falg)++;
        //printf("----test_timeout----\n");
    }
    //printf("------OnTimeout-----------\n");
}

QueryStatus NXLHttpClientAsync::Run(){
    try
    {
        tcp::resolver resolver_{ m_ioService };
        boost::system::error_code ec;
        auto const lookup = resolver_.resolve(m_strHost.c_str(), m_strPort.c_str(), ec);
        if (ec){
            return QS_E_ResolveFailed;
        }
          
        //deadline_timer
        int timeout_flag = 0;
        boost::asio::deadline_timer timer(m_ioService);
        timer.expires_from_now(boost::posix_time::milliseconds(m_msTimeout));
        timer.async_wait(boost::bind(on_timer_static,boost::asio::placeholders::error,&timeout_flag));

        //
        Clear();
        //
        _req->set(http::field::host, m_strHost.c_str());

        #if 0
            tcp::resolver resolver_{ m_ioService };
            resolver_.async_resolve(
                    m_strHost.c_str(),
                    m_strPort.c_str(),
                    std::bind(
                            &NXLHttpClientAsync::OnResolve,
                            shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2));
        #else
            if (m_https)  {
                m_pHttpsStream = new ssl::stream<tcp::socket>{m_ioService, m_SSLCtx};
                
                SSL* pSSL = m_pHttpsStream->native_handle();
                SSL_set_tlsext_host_name(pSSL, m_strHost.c_str());

                boost::asio::async_connect(m_pHttpsStream->next_layer(), lookup,
                                        boost::bind(&NXLHttpClientAsync::OnConnect, shared_from_this(), boost::asio::placeholders::error)); //
            }
            else {
                m_pHttpSocket = new tcp::socket(m_ioService);
                boost::asio::async_connect(*m_pHttpSocket, lookup,
                                        boost::bind(&NXLHttpClientAsync::OnConnect, shared_from_this(), boost::asio::placeholders::error));
            }

        #endif
        //
        m_ioService.reset();
        //int n = 0;
        while (m_ioService.run_one())
        {
            //n++;
            if (m_useBreak  ) {
                timer.cancel();
               // printf("testx --%d----succeed-----------\n",timeout_flag);
                break;
            } else if(timeout_flag>0){
               // printf("testx --%d----timeout-----------\n",timeout_flag);
                break;
            } else {
                //printf("testx -%d-\n",timeout_flag);
            }
        }
        //
        m_ioService.stop();
        Clear();
        //

        if(m_resolveFailed){
            // printf("testx m_resolveFailed\n");
            return QueryStatus::QS_E_ServiceUnavailable;
        }
        if(m_connectFailed){
           // printf("testx m_connectFailed\n");
            return QueryStatus::QS_E_DisConnect;
        }
        if(m_writeFailed){
            //printf("testx m_writeFailed\n");
            return QueryStatus::QS_E_ReadFailed;
        }
        if(m_writeFailed){
            //printf("testx m_writeFailed\n");
            return QueryStatus::QS_E_WriteFailed;
        }
        if(timeout_flag>0){
            //printf("testx timeout_flag\n");
            return QueryStatus::QS_E_Timeout;
        }
        //printf("testx QS_S_OK\n");
        return QS_S_OK;
    }
    catch (...)
    {
        //printf("testx %s exception:%s\n", __func__, ex.what() );
    }
    return QueryStatus::QS_E_Failed;
}


QueryStatus NXLHttpClientAsync::RunSync(){
    try
    {
        tcp::resolver resolver{ m_ioService };
        auto const lookup = resolver.resolve({ m_strHost.c_str(), m_strPort.c_str()});
        //
        Clear();
        //
        _req->set(http::field::host, m_strHost.c_str());
        //connect
        if(m_https){
            m_pHttpsStream = new ssl::stream<tcp::socket>{ m_ioService, m_SSLCtx};
            boost::asio::connect(m_pHttpsStream->next_layer(), lookup);
            // Perform the SSL handshake
            m_pHttpsStream->handshake(ssl::stream_base::client);
            //get http header
            _req->set(http::field::host, m_strHost.c_str());
            //
            http::write(*m_pHttpsStream, *_req);
            //
            http::read(*m_pHttpsStream, *_flat_buffer, *_res);
        } else {
            m_pHttpSocket = new tcp::socket(m_ioService);
            boost::asio::connect(*m_pHttpSocket,       lookup);
            //
            http::write((*m_pHttpSocket), *_req);
            //
            http::read(*m_pHttpSocket, *_flat_buffer, *_res);
        }
        //

        return QS_S_OK;
    }
    catch (...)
    {
        //debug Log
    }
    return QueryStatus::QS_E_Failed;
}


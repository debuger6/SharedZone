#include "SharedServer.h"
#include "SharedSession.h"
#include "Types.h"

#include <muduo/base/Logging.h>

#include <boost/bind.hpp>

// using namespace muduo;
// using namespace muduo::net;

SharedServer::SharedServer(muduo::net::EventLoop* loop,
		                       const muduo::net::InetAddress& listenAddr)
	  : loop_(loop),
	    server_(loop, listenAddr, "SharedServer")
{
	 server_.setConnectionCallback(
			        boost::bind(&SharedServer::onConnection, this, _1));
	 server_.setMessageCallback(
				    boost::bind(&SharedServer::onMessage, this, _1, _2, _3));
}

void SharedServer::start()
{
	 server_.start();
}

void SharedServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
	 if (conn->connected())
     {
		 conn->setContext(SharedSession());	// TcpConnection与一个SharedSession绑定
	 }
}

void SharedServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
		                           muduo::net::Buffer* buf, muduo::Timestamp time)
{
	while (buf->readableBytes() >= kHeaderLen)
	{
		const void* data = buf->peek();
		const RequestHead* rh = static_cast<const RequestHead*>(data);
	    uint16 len = muduo::net::sockets::networkToHost16(rh->len); 
		if (buf->readableBytes() >= len + kHeaderLen)  // 达到一条完整的消息
		{
			SharedSession* ss = boost::any_cast<SharedSession>(conn->getMutableContext()); //return context
			ss->SetData(buf->peek(), kHeaderLen + len);
			ss->Process();
			muduo::net::Buffer response;
			response.append(ss->GetJos().Data(), ss->GetJos().Length());
			ss->Clear();
			conn->send(&response);

			buf->retrieve(kHeaderLen+len);
		}

		else{
			break;
		}
	}
}



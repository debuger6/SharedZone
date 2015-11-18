#ifndef SHARED_SERVER_H
#define SHARED_SERVER_H

#include <muduo/net/TcpServer.h>

class SharedServer : boost::noncopyable
{
public:
	SharedServer(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr);
	void start(); //will call server_.start()

private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);

	void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

private:
	muduo::net::EventLoop* loop_;
	muduo::net::TcpServer server_;
	const static size_t kHeaderLen = 4;  //请求包头4个字节 cmd(2)+len(2)
};

#endif


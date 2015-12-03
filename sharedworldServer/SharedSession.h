#ifndef _SHARED_SESSION_H_
#define _SHARED_SESSION_H_

#include <muduo/base/Logging.h>
#include <muduo/net/TcpServer.h>
#include "Types.h"
#include "JInStream.h"
#include "JOutStream.h"
//#include "../Public/JUtil.h"
//#include "../Public/Socket.h"
#include "MD5.h"
#include <map>
#include "Idea.h"
#include <memory>
#include "./dal/SharedService.h"
#include <iostream>
using namespace std;

#define CMD_LOGIN					0x01
#define CMD_REGISTER			    0x02
#define CMD_DEPOSIT					0x03
#define CMD_WITHDRAW				0x04
#define CMD_TRANSFER				0x05
#define CMD_BALANCE_INQUIRY			0x06
#define CMD_CHANGE_PASSWORD			0x07
#define CMD_DAY_BILL				0x08
#define CMD_HISTORY_BILL			0x09
#define CMD_ACCOUNT_HISTORY_BILL	0x0A
#define CMD_CLOSE_ACCOUNT			0x0B

#define CMD_USERLIST				0X10
#define CMD_SEND_CONTENT			0x11
#define CMD_SEND_REBACK				0x12

struct RequestHead
{
	unsigned short cmd;
	unsigned short len;
};

struct ResponseHead
{
	//unsigned short为两个字节16位
	unsigned short cmd;
	unsigned short len;
	unsigned short cnt;
	unsigned short seq;
	unsigned short error_code;
	char error_msg[30];
};

struct RequestPack
{
	RequestHead head;
	char buf[1];
};

struct ResponsePack
{
	ResponseHead head;
	char buf[1];
};

class SharedSession
{
public:
	SharedSession(map<string, muduo::net::TcpConnectionPtr>& conns, const muduo::net::TcpConnectionPtr  conn);
	~SharedSession();

	void SetData(const char* data, size_t len);
	void Process();
	uint16 GetCmd() const { return requestPack_->head.cmd; }
	RequestPack* GetRequestPack() const { return requestPack_; }
	void Send(const char* buf, size_t len);
	void Recv();
	void DoAction();

	JOutStream& GetJos() { return jos_; }
	void Clear() { jos_.Clear(); josres_.Clear(); }

	string getAccount(){return account_;}
    void setAccount(string account){account_ = account;}

	void removeActiveUser();
	void removeConn(const muduo::net::TcpConnectionPtr conn);

	void setSessionType(string type){sessionType_ = type;}
	string getSessionType(){return sessionType_;}
	JOutStream& GetJosres(){return josres_;}

private:
	void Parse();
	char buffer_[2048];
	RequestPack* requestPack_;
	JOutStream jos_;
	string account_;

	JOutStream josres_;
	string sessionType_;
public:
	map<string, muduo::net::TcpConnectionPtr>& conns_;
	const muduo::net::TcpConnectionPtr conn_;
};


#endif // _SHARED_SESSION_H_

#include "SharedSession.h"

#include "TransactionManager.h"

#include <muduo/net/Endian.h>
#include <muduo/base/Exception.h>
#include <muduo/base/Singleton.h>

#include <stdio.h>
SharedSession::SharedSession()
{
}

SharedSession::~SharedSession()
{
}

void SharedSession::SetData(const char* data, size_t len)
{
	memcpy(buffer_, data, len);
}

void SharedSession::Process()
{
	Parse();
	//DoAction();
	
	try
	{
		DoAction();
	}
	catch (muduo::Exception& e)
	{
		LOG_INFO<<e.what();
	}
	
}

/*
void SharedSession::Send(const char* buf, size_t len)
{
	socket_->SendN(buf, len);
}
*/

void SharedSession::Parse()
{
	requestPack_= (RequestPack*)(buffer_);
	uint16 cmd = muduo::net::sockets::networkToHost16(requestPack_->head.cmd);
	uint16 len = muduo::net::sockets::networkToHost16(requestPack_->head.len);
	
	// ����hash
	unsigned char hash[16];
	MD5 md5;
	md5.MD5Make(hash, reinterpret_cast<unsigned char const *>(buffer_), sizeof(RequestHead)+len-8);
	for (int i=0; i<8; ++i)
	{
		hash[i] = hash[i] ^ hash[i+8];

		//unsigned char��ת���ԡ�int��ʱ���ܸı�ֵ

		hash[i] = hash[i] ^ (static_cast<unsigned char>((cmd >> (i%2)) & 0xff));
	}

	if (memcmp(hash, buffer_+sizeof(RequestHead)+len-8, 8))
		throw muduo::Exception("��������ݰ�");

	requestPack_->head.cmd = cmd;
	requestPack_->head.len = len;

}

void SharedSession::DoAction()
{
	muduo::Singleton<TransactionManager>::instance().DoAction(*this);  //ʵ����һ��TransactionManager�������󣬲�����TransactionManager��DoAction(..)
}


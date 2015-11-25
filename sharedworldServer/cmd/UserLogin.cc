#include "UserLogin.h"
#include <string>
#include "../SharedSession.h"
#include <iostream>
#include "../dal/SharedService.h"
using namespace std;

void UserLogin::Execute(SharedSession& session)
{
	
	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 柜员登录名
	string name;
	jis>>name;
	session.setAccount(name);
	// 密码
	char pass[16];
	unsigned char ideaKey[16];
	unsigned char buf[2];
	buf[0] = (cmd >> 8) & 0xff;
	buf[1] = cmd & 0xff;
	MD5 md5;
	md5.MD5Make(ideaKey, buf, 2);
	for (int i=0; i<8; ++i)
	{
		ideaKey[i] = ideaKey[i] ^ ideaKey[i+8];
		ideaKey[i] = ideaKey[i] ^ ((cmd >> (i%2)) & 0xff);
		ideaKey[i+8] = ideaKey[i] ^ ideaKey[i+8];
		ideaKey[i+8] = ideaKey[i+8] ^ ((cmd >> (i%2)) & 0xff);
	}
	char encryptedPass[16];
	jis.ReadBytes(encryptedPass, 16);
	Idea idea;
	// 解密
	idea.Crypt(ideaKey, (const unsigned char*)encryptedPass, (unsigned char *)pass, 16, false);

	int16 error_code = 0;
	char error_msg[31] = {0};


	// 实际的登录操作
	SharedService dao;
	list<string> activeUsers;
	int ret;
	ret = dao.UserLogin(name, pass, activeUsers);
	JOutStream& jos = session.GetJos();
	JOutStream& josres = session.GetJosres();

	if (ret == 0)
	{
		LOG_INFO<<"登录成功";
		cout<<"ss addr:"<<&session<<endl;
		session.conns_.insert(make_pair<string, muduo::net::TcpConnectionPtr>(name, session.conn_));
		cout<<"conns3:"<<&session.conns_<<endl;
		cout<<"conn3:"<<session.conn_<<endl;
		cout<<"session.conns_ size:"<< session.conns_.size()<<endl;
		uint16 cnt = static_cast<uint16>(activeUsers.size());
		uint16 seq = 0;
		list<string>::const_iterator it;
		
		uint16 cmd_res = 0x10;
		josres<<cmd_res;
		size_t lengthPos = josres.Length();	// len位置
		josres.Skip(2);			// 为len预留两个字节
		for (it=activeUsers.begin(); it!=activeUsers.end(); ++it)
		{
			josres<<*it;
		}
		{
			size_t tailPos = josres.Length();
			josres.Reposition(lengthPos);
			josres<<(uint16)(tailPos + 8 - sizeof(ResponseHead));	// 包体+包尾长度

			// 包尾
			josres.Reposition(tailPos);
			unsigned char hash[16];
			MD5 md5;
			md5.MD5Make(hash, (unsigned char const*)josres.Data(), josres.Length());
			for (int i=0; i<8; ++i)
			{
				hash[i] = hash[i] ^ hash[i+8];						
				hash[i] = hash[i] ^ ((cmd_res >> (i%2)) & 0xff);
																										
			}
			josres.WriteBytes(hash, 8);
		}

		
		for (it=activeUsers.begin(); it!=activeUsers.end(); ++it)
		{
			JOutStream josTmp;
			// 包头cmd+len+cnt+seq+error_code+error_msg
			josTmp<<cmd;
			size_t lengthPos = josTmp.Length();	// len位置
			josTmp.Skip(2);			// 为len预留两个字节
			josTmp<<cnt<<seq<<error_code;
			seq++;
			josTmp.WriteBytes(error_msg, 30);

			//包体account
			josTmp<<*it;

			// 包头len字段
			size_t tailPos = josTmp.Length();
			josTmp.Reposition(lengthPos);
			josTmp<<(uint16)(tailPos + 8 - sizeof(ResponseHead));	// 包体+包尾长度
			
			// 包尾
			josTmp.Reposition(tailPos);
			unsigned char hash[16];
			MD5 md5;
			md5.MD5Make(hash, (unsigned char const*)josTmp.Data(), josTmp.Length());
			for (int i=0; i<8; ++i)
			{
				hash[i] = hash[i] ^ hash[i+8];
				hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
																										
			}
			josTmp.WriteBytes(hash, 8);
			jos.WriteBytes(josTmp.Data(), josTmp.Length());

		}
		return;
	}
	else if (ret == 1)
	{
		error_code = 1;
		strcpy(error_msg, "用户名或密码错误");
		LOG_INFO<<error_msg;
	}
	else if (ret == -1)
	{
		error_code = -1;
		strcpy(error_msg, "数据库错误");
		LOG_INFO<<error_msg;
	}

	// 包头命令
	jos<<cmd;
	size_t lengthPos = jos.Length();
	jos.Skip(2);
	// 包头cnt、seq、error_code、error_msg
	uint16 cnt = 0;
	uint16 seq = 0;
	jos<<cnt<<seq<<error_code;
	jos.WriteBytes(error_msg, 30);
	// 包体为空

	// 包头len
	size_t tailPos = jos.Length();
	jos.Reposition(lengthPos);
	jos<<static_cast<uint16>(tailPos + 8 - sizeof(ResponseHead)); // 包体长度 + 包尾长度

	// 包尾
	jos.Reposition(tailPos);
	// 计算包尾
	unsigned char hash[16];
	md5.MD5Make(hash, (const unsigned char*)jos.Data(), jos.Length());
	for (int i=0; i<8; ++i)
	{
		hash[i] = hash[i] ^ hash[i+8];
		hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
	}
	jos.WriteBytes(hash, 8);

	//session.Send(jos.Data(), jos.Length());
	
}


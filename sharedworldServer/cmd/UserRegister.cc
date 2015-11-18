#include "UserRegister.h"
#include <string>
#include "../SharedSession.h"
#include <iostream>
#include "../dal/SharedService.h"
using namespace std;

void UserRegister::Execute(SharedSession& session)
{
	
	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 注册的登录名
	string name;
	jis>>name;
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


	// 实际的注册操作
	Account acc;
	acc.account = name;
	acc.pass = pass;
	SharedService dao;
	int ret;
    ret = dao.UserRegister(acc);


	if (ret == 0)
	{
		LOG_INFO<<"注册成功";
	}
	else if (ret == 1)
	{
		error_code = 1;
		strcpy(error_msg, "用户名已存在");
		LOG_INFO<<error_msg;
	}
	else if (ret == -1)
	{
		error_code = -1;
		strcpy(error_msg, "数据库错误");
		LOG_INFO<<error_msg;
	}

	JOutStream& jos = session.GetJos();
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


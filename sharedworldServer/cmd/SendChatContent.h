#ifndef _SEND_CHAT_CONTENT_H_
#define _SEND_CHAT_CONTENT_H_

#include "Transaction.h"

// 用户发送聊天信息到公告面板
class SendChatContent : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif 

#ifndef _SEND_CHAT_CONTENT_H_
#define _SEND_CHAT_CONTENT_H_

#include "Transaction.h"

// �û�����������Ϣ���������
class SendChatContent : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif 

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "../SharedSession.h"


// �������
class Transaction
{
public:
	virtual void Execute(SharedSession& session) = 0;
	virtual ~Transaction() {};
};


#endif // _TRANSACTION_H_

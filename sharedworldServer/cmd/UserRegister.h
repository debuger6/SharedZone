#ifndef _USER_REGISTER_H_
#define _USER_REGISTER_H_

#include "Transaction.h"

// ��Ա��¼
class UserRegister : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif // _USER_REGISTER_H_

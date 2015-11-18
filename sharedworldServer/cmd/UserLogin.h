#ifndef _USER_LOGIN_H_
#define _USER_LOGIN_H_

#include "Transaction.h"

// ¹ñÔ±µÇÂ¼
class UserLogin : public Transaction
{
public:
	virtual void Execute(SharedSession& session);
};



#endif // _USER_LOGIN_H_

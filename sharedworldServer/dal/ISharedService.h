#ifndef _ISHARED_SERVICE_H_
#define _ISHARED_SERVICE_H_

#include "Account.h"
#include "TransDetail.h"

#include <list>
using namespace std;

class ISharedService
{
public:
	// 用户登录
	virtual int UserLogin(const string& user, const string& pass, list<string>& activeUsers) = 0;
	// 注册
	virtual int UserRegister(Account& account) = 0;
	// 修改密码
	virtual int ChangePassword(Account& account, const string& newPass) = 0;
	// 查询某日报表
	virtual int QueryDayBill(list<TransDetail>& result, int page, const string& date) = 0;
	// 根据日期查询明细
	virtual int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end) = 0;
	// 根据帐号查询明细
	virtual int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end) = 0;
};

#endif /* _ISHARED_SERVICE_H_ */

#ifndef _SHARED_SERVICE_H_
#define _SHARED_SERVICE_H_

#include "ISharedService.h"

class SharedService : public ISharedService
{
public:
	// 用户登录
	int UserLogin(const string& user, const string& pass);
	// 注册
	int UserRegister(Account& account);
	// 修改密码
	int ChangePassword(Account& account, const string& newPass);
	// 查询某日报表
	int QueryDayBill(list<TransDetail>& result, int page, const string& date);
	// 根据日期查询明细
	int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end);
	// 根据帐号查询明细
	int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end);
};

#endif /* _SHARED_SERVICE_H_ */

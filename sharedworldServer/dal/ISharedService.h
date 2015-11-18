#ifndef _ISHARED_SERVICE_H_
#define _ISHARED_SERVICE_H_

#include "Account.h"
#include "TransDetail.h"

#include <list>
using namespace std;

class ISharedService
{
public:
	// �û���¼
	virtual int UserLogin(const string& user, const string& pass, list<string>& activeUsers) = 0;
	// ע��
	virtual int UserRegister(Account& account) = 0;
	// �޸�����
	virtual int ChangePassword(Account& account, const string& newPass) = 0;
	// ��ѯĳ�ձ���
	virtual int QueryDayBill(list<TransDetail>& result, int page, const string& date) = 0;
	// �������ڲ�ѯ��ϸ
	virtual int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end) = 0;
	// �����ʺŲ�ѯ��ϸ
	virtual int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end) = 0;
};

#endif /* _ISHARED_SERVICE_H_ */

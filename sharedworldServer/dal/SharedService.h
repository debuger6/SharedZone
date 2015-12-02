#ifndef _SHARED_SERVICE_H_
#define _SHARED_SERVICE_H_

#include "ISharedService.h"
#include "../JOutStream.h"
class SharedService : public ISharedService
{
public:
	// �û���¼
	int UserLogin(const string& user, const string& pass, list<string>& activeUsers);
	// ע��
	int UserRegister(Account& account);
	//�˳�
	int LogOut(string account, JOutStream& jos, int len);
	// �޸�����
	int ChangePassword(Account& account, const string& newPass);
	// ��ѯĳ�ձ���
	int QueryDayBill(list<TransDetail>& result, int page, const string& date);
	// �������ڲ�ѯ��ϸ
	int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end);
	// �����ʺŲ�ѯ��ϸ
	int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end);
};

#endif /* _SHARED_SERVICE_H_ */

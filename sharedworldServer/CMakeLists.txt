set(sharedserver_SRCS
  main.cc
  SharedServer.cc
  SharedSession.cc
  cmd/BalanceInquiry.cc
  cmd/ChangePassword.cc
  cmd/CloseAccount.cc
  cmd/Deposit.cc
  cmd/OpenAccount.cc
  cmd/QueryAccountHistoryBill.cc
  cmd/QueryDayBill.cc
  cmd/QueryHistoryBill.cc
  cmd/Transfer.cc
  cmd/UserLogin.cc
  cmd/Withdrawal.cc
  dal/SharedService.cc
  dal/MysqlDB.cc
  Idea.cc
  JInStream.cc
  JOutStream.cc
  JUtil.cc
  MD5.cc
  Server.cc
  StringUtil.cc
  SystemConfig.cc
  TransactionManager.cc
  )

add_executable(sharedserver ${sharedserver_SRCS})
target_link_libraries(sharedserver muduo_net mysqlclient z m)

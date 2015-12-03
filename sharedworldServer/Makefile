MUDUO_DIRECTORY ?= /home/xiezhenyu/share/muduo-1.0.6/build/release-install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib

CXXFLAGS = -g -O0 -Wall -Wextra \
	-Wconversion -Wno-unused-parameter \
	-Wold-style-cast -Woverloaded-virtual \
	-Wpointer-arith -Wshadow -Wwrite-strings \
	-march=native -rdynamic \
	-I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lrt -lmysqlclient

VPATH=src:./cmd:./dal
objects=main.o Server.o SharedServer.o SharedSession.o TransactionManager.o UserLogin.o Server.o Idea.o JInStream.o JOutStream.o StringUtil.o SystemConfig.o MD5.o UserRegister.o MysqlDB.o SharedService.o JUtil.o SendChatContent.o
MyServer:$(objects)
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
main.o:
Server.o:
SharedServer.o:
SharedSession.o:
TransactionManager.o:
UserLogin.o:
UserRegister.o:
Server.o:
Idea.o:
JInStream.o:
JOutStream.o:
StringUtil.o:
SystemConfig.o:
MD5.o:
MysqlDB.o:
SharedService.o:
JUtil.o:

.PHONY:clean
clean:
	-rm  $(objects)

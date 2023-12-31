#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include "../Common/PacketDefine.h"

class SocketIocpController;

class IOCPChatServer
{
public:
	IOCPChatServer();
	~IOCPChatServer();

	bool initialize(const UINT32 maxIOThreadCount);
	bool bindAndListen(const int bindPort);
	bool run(const UINT32 maxClientCount);
	void shutdown();

private:
	SocketIocpController* getAvailableSocketIocpController();

	void createWorkThread();
	void workThreadMain();

	void closeSocketIocpControllerAndStartAccept(SocketIocpController& socketIocpController, bool isForce = false);
	void sendMsgAllClients(const std::string& msgStirng);

private:
	std::vector<std::thread>		_workThreads;
	//채팅을 보낼 때 반복해서 순회해야하기 때문에 vector로 결정
	std::vector<std::unique_ptr<SocketIocpController>>	_socketIocpControllers;
	std::mutex						_socketIocpControllersLock;
	WSADATA							_wsaData;
	SOCKET							_serverSock;
	HANDLE							_iocpHandle			= nullptr;
	UINT32							_maxIOThreadCount	= 0;
	UINT32							_maxClientCount		= 0;
	bool							_wsaStartupResult	= false;
	bool							_isWorkThreadRun	= false;
};
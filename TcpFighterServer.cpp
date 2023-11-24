#include "stdafx.h"
#include"CRingBuffer.h"
#include "Session.h"
#include "CGlobal.h"
#include "CServer.h"
#include "CLogic.h"
using namespace std;

DWORD startTime;
DWORD addTime;
DWORD endTime; //unsigned int 
DWORD idealTime;
DWORD Time;

int main()
{
	timeBeginPeriod(1);


	int iCount;
	for (iCount = 0; iCount < 63; iCount++) {
		CGlobal::sessionArray[iCount].clientSock = INVALID_SOCKET;
	}

	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 0;
	}


	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET) {
		cout << "socket" << WSAGetLastError();
		return 0;
	}


	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR) {
		cout << "bind" << WSAGetLastError();
	}


	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		cout << "listen" << WSAGetLastError();
	}


	u_long on = 1;
	retval = ioctlsocket(listenSock, FIONBIO, &on);
	if (retval == SOCKET_ERROR) {
		cout << "논 블로킹" << WSAGetLastError();
		return 0;
	}

	int addrlen;

	FD_SET rset;
	FD_SET wset;


	SOCKET acceptClientSock;

	startTime = timeGetTime();
	Time = timeGetTime();
	while (1) {
		int selectRet;

		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(listenSock, &rset);


		int jCount;
		for (jCount = 0; jCount < 63; jCount++) {
			if (CGlobal::sessionArray[jCount].clientSock == INVALID_SOCKET)continue;
			FD_SET(CGlobal::sessionArray[jCount].clientSock, &rset);
			FD_SET(CGlobal::sessionArray[jCount].clientSock, &wset);
		}


		TIMEVAL tv{ 0, 0 };
		selectRet = select(0, &rset, &wset, NULL, &tv);
		if (selectRet == SOCKET_ERROR) {
			cout << "select" << WSAGetLastError();
		}
		if (FD_ISSET(listenSock, &rset)) {

			if (CGlobal::cnt < 63) {
				addrlen = sizeof(CGlobal::clientAddr);
				acceptClientSock = accept(listenSock, (SOCKADDR*)&CGlobal::clientAddr, &addrlen);
				if (acceptClientSock == INVALID_SOCKET) {
					cout << "acc" << WSAGetLastError();
				}
				//세션 추가하기 
				CServer::procAccept(acceptClientSock);
				CGlobal::id++;
				CGlobal::cnt++;
			}
		}
		int iCount;
		for (iCount = 0; iCount < 63; iCount++) {
			if (CGlobal::sessionArray[iCount].clientSock == INVALID_SOCKET)continue;
			if (FD_ISSET(CGlobal::sessionArray[iCount].clientSock, &rset)) {
				//recv
				CServer::procRecv(&CGlobal::sessionArray[iCount]);
			}
		}


		CLogic::update();

		int pCount;
		for (pCount = 0; pCount < 63; pCount++) {
			if (CGlobal::sessionArray[pCount].clientSock == INVALID_SOCKET) {
				continue;
			}
			if (FD_ISSET(CGlobal::sessionArray[pCount].clientSock, &wset)) {
				//send
				CServer::procSendPacket(&CGlobal::sessionArray[pCount]);

			}
		}
		idealTime = startTime + addTime;
		addTime += 20;
		endTime = timeGetTime();
		if (endTime - idealTime < 20) {
			Sleep(20 - (endTime - idealTime));
		}

	}

	return 0;
}

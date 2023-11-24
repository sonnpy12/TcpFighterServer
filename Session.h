#pragma once
#include "CRingBuffer.h"

#define SERVERPORT 5000

struct stSession {
	int id;
	char moveDirection;
	char attackDirection;
	short x;
	short y;
	char hp;
	bool isMove;
	SOCKET clientSock;
	USHORT port;
	IN_ADDR ip;
	CRingBuffer recvQueue;
	CRingBuffer sendQueue;
};

#pragma once
#include "CGlobal.h"
#include "CProtocol.h"
class CServer {

public:
	static void procAccept(SOCKET acceptClientSock);
	static void procRecv(stSession* session);
	static void procRecvPacket(stSession* session);
	static void procSendPacket(stSession* session);
	static void sendUniCast(stSession* session, char* message, int messageSize, int type);
	static void sendBroadCast(stSession* session, char* message, int messageSize, int type);
	static void createMyCharacter(stSession* session);
	static void createOtherPlayer(stSession* session);
	static void disconnectPlayer(stSession* session);

	//패킷 핸들 메세지 
	static void handle_PACKET_C_MOVE_START(stSession* session);
	static void handle_PACKET_C_MOVE_STOP(stSession* session);
	static void handle_PACKET_C_MOVE_ATTACK1(stSession* session);
	static void handle_PACKET_C_MOVE_ATTACK2(stSession* session);
	static void handle_PACKET_C_MOVE_ATTACK3(stSession* session);
	static void conflictAttack(stSession* session, short attackRangeX, short attackRange);
	static bool isConflict(stSession* session, stSession* target, short attackRangeX, short attackRangeY);
	static bool moveError(stSession* session, short packetX, short packetY);


};



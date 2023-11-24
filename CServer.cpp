#include "stdafx.h"
#include "CServer.h"
#include "PacketDefine.h"

void CServer::procAccept(SOCKET acceptClientSock) {

	int iCount;
	for (iCount = 0; iCount < 63; iCount++) {
		if (CGlobal::sessionArray[iCount].clientSock == INVALID_SOCKET) {

			//세션 정리 그리고 추가 
			CGlobal::sessionArray[iCount].clientSock = acceptClientSock;
			CGlobal::sessionArray[iCount].ip = CGlobal::clientAddr.sin_addr;
			CGlobal::sessionArray[iCount].port = CGlobal::clientAddr.sin_port;

			//server->client 내 캐릭터 추가하기 (uniCast)
			CServer::createMyCharacter(&CGlobal::sessionArray[iCount]);
			//server->client 다른 캐릭터 추가하기 (broadCast)
			CServer::createOtherPlayer(&CGlobal::sessionArray[iCount]);
			break;
		}
	}


}
void CServer::createMyCharacter(stSession* session) {


	//패킷 메세지 만들기
	CProtocol::stPACKET_S_CREATE_MY_CHARACTER createMyCharacter;

	createMyCharacter.id = CGlobal::id;
	createMyCharacter.moveDirection = dfPACKET_MOVE_DIR_RR;
	createMyCharacter.hp = 100;
	createMyCharacter.x = 100;
	createMyCharacter.y = 100;

	//세션 초기화
	session->id = CGlobal::id;
	session->x = createMyCharacter.x;
	session->y = createMyCharacter.y;
	session->hp = createMyCharacter.hp;
	session->moveDirection = createMyCharacter.moveDirection;
	session->isMove = false;


	//유니캐스트로 내 세션 링버퍼에 인큐
	CServer::sendUniCast(session, (char*)&createMyCharacter, sizeof(createMyCharacter), dfPACKET_SC_CREATE_MY_CHARACTER);
	//브로드캐스트로 다른 세션 링버퍼에 인큐
	CServer::sendBroadCast(session, (char*)&createMyCharacter, sizeof(createMyCharacter), dfPACKET_SC_CREATE_OTHER_CHARACTER);
}
void CServer::createOtherPlayer(stSession* session) {

	int iCount;
	for (iCount = 0; iCount < 63; iCount++) {
		if (CGlobal::sessionArray[iCount].clientSock == INVALID_SOCKET) {
			continue;
		}
		if (session->clientSock == CGlobal::sessionArray[iCount].clientSock) {
			continue;
		}
		CProtocol::stPACKET_S_CHREATE_OTHER_CHARACTER createOtherCharacter;
		createOtherCharacter.id = CGlobal::sessionArray[iCount].id;
		createOtherCharacter.moveDirection = CGlobal::sessionArray[iCount].moveDirection;
		createOtherCharacter.x = CGlobal::sessionArray[iCount].x;
		createOtherCharacter.y = CGlobal::sessionArray[iCount].y;
		createOtherCharacter.hp = CGlobal::sessionArray[iCount].hp;
		//유니캐스트로 
		CServer::sendUniCast(session, (char*)&createOtherCharacter, sizeof(createOtherCharacter), dfPACKET_SC_CREATE_OTHER_CHARACTER);

	}

}

void CServer::procRecv(stSession* session) {

	int recvRet;

	int freeSize;
	freeSize = session->recvQueue.GetFreeSize();
	if (freeSize == 0) {
		return;
	}
	char* recvBuf = new char[freeSize];

	recvRet = recv(session->clientSock, recvBuf, freeSize, 0);
	if (recvRet == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return;
		}
		else {
			cout << WSAGetLastError();
			//연결 끊기 
			CServer::disconnectPlayer(session);
		}
	}
	//연결이 정상적으로 닫힌 경우 반환값이 0
	else if (recvRet == 0) {
		//연결 끊기
		CServer::disconnectPlayer(session);
	}
	else {

		//recvQueue에 인큐 하기 
		session->recvQueue.Enqueue(recvBuf, recvRet);
		//패킷 처리 함수 만들기
		CServer::procRecvPacket(session);
	}

	delete[] recvBuf;
}
void CServer::procRecvPacket(stSession* session) {

	//먼저 패킷 헤더를 뺄 수 있는 사이즈인지 확인하기 
	int recvUseSize = session->recvQueue.GetUseSize();
	CProtocol::stPacketHeader header;
	//패킷 헤더 완성 여부 확인
	if (recvUseSize < 3) {
		return;
	}
	session->recvQueue.Peek((char*)&header, 3);
	if (header.byCode != 0x89) {
		return;
	}
	//메세지 사이즈, 타입 확인
	int packetSize = header.bySize;
	int  packetType = header.byType;

	//하나의 메세지 완성 여부 확인
	session->recvQueue.MoveFront(3);
	recvUseSize = session->recvQueue.GetUseSize();
	if (recvUseSize < packetSize) {
		return;
	}
	CGlobal::recvCount++;
	switch (packetType)
	{

	case dfPACKET_CS_MOVE_START:
	{
		cout << "[RECV]" << "CS_MOVE_START_HEADER" << " ";
		cout << (int)header.byCode << " / " << "size: " << (int)header.bySize << " " << "type: " << (int)header.byType;
		CServer::handle_PACKET_C_MOVE_START(session);

		break;
	}
	case dfPACKET_CS_MOVE_STOP:
	{
		cout << "[RECV]" << "CS_MOVE_STOP_HEADER" << " ";
		cout << (int)header.byCode << " / " << "size: " << (int)header.bySize << " / " << "type: " << (int)header.byType;
		CServer::handle_PACKET_C_MOVE_STOP(session);
		break;
	}
	case dfPACKET_CS_ATTACK1:
	{
		cout << "[RECV]" << "CS_MOVE_ATTACK1_HEADER" << " ";
		cout << (int)header.byCode << " / " << "size: " << (int)header.bySize << " / " << "type: " << (int)header.byType;
		CServer::handle_PACKET_C_MOVE_ATTACK1(session);
		break;
	}
	case dfPACKET_CS_ATTACK2:
	{
		cout << "[RECV]" << "CS_MOVE_ATTACK2_HEADER" << " ";
		cout << (int)header.byCode << " / " << "size: " << (int)header.bySize << " / " << "type: " << (int)header.byType;
		CServer::handle_PACKET_C_MOVE_ATTACK2(session);
		break;
	}
	case dfPACKET_CS_ATTACK3:
	{
		cout << "[RECV]" << "CS_MOVE_ATTACK3_HEADER" << " ";
		cout << (int)header.byCode << " /  " << "size: " << (int)header.bySize << " / " << "type: " << (int)header.byType;
		CServer::handle_PACKET_C_MOVE_ATTACK3(session);
		break;
	}
	default:
		break;
	}



}
void CServer::procSendPacket(stSession* session)
{
	int sendRet;

	int sendUseSize;
	sendUseSize = session->sendQueue.GetUseSize();
	if (sendUseSize == 0) {
		return;
	}
	char* sendBuf = new char[sendUseSize];
	session->sendQueue.Peek(sendBuf, sendUseSize);

	sendRet = send(session->clientSock, sendBuf, sendUseSize, 0);
	session->sendQueue.MoveFront(sendRet);

	delete[] sendBuf;
}
void CServer::handle_PACKET_C_MOVE_START(stSession* session) {

	//디큐
	CProtocol::stPACKET_C_MOVE_START clientMoveStart;
	session->recvQueue.Dequeue((char*)&clientMoveStart, sizeof(clientMoveStart));
	cout << "[MESSAGE] " << "_C_MOVE_START" << " " << "SESSION ID: " << session->id << " / ";
	cout << "Direction: " << (short)clientMoveStart.moveDirection << " / " << clientMoveStart.x << " " << clientMoveStart.y << "\n";

	//이동 오류 체크 범위 
	bool resultMoveError = CServer::moveError(session, clientMoveStart.x, clientMoveStart.y);
	if (resultMoveError == true) {
		CServer::disconnectPlayer(session);
		return;
	}

	//세션 업데이트
	session->moveDirection = clientMoveStart.moveDirection;
	session->x = clientMoveStart.x;
	session->y = clientMoveStart.y;
	session->isMove = true;


	//SERVER메세지 만들어서 브로드 캐스트
	CProtocol::stPACKET_S_MOVE_START serverMoveStart;
	serverMoveStart.id = session->id;
	serverMoveStart.moveDirection = session->moveDirection;
	serverMoveStart.x = session->x;
	serverMoveStart.y = session->y;

	CServer::sendBroadCast(session, (char*)&serverMoveStart, sizeof(serverMoveStart), dfPACKET_SC_MOVE_START);



}
void CServer::handle_PACKET_C_MOVE_STOP(stSession* session) {

	//디큐
	CProtocol::stPACKET_C_MOVE_STOP clientMoveStop;
	session->recvQueue.Dequeue((char*)&clientMoveStop, sizeof(clientMoveStop));
	cout << "[MESSAE] " << "_C_MOVE_STOP" << " " << "SESSION ID: " << session->id << " / ";
	cout << "Direction: " << (short)clientMoveStop.moveDirection << " / " << clientMoveStop.x << " " << clientMoveStop.y << "\n";

	//이동범위 오류체크
	bool resultMoveError = CServer::moveError(session, clientMoveStop.x, clientMoveStop.y);
	if (resultMoveError == true) {
		CServer::disconnectPlayer(session);
		return;
	}

	//세션 업데이트 
	session->moveDirection = clientMoveStop.moveDirection;
	session->x = clientMoveStop.x;
	session->y = clientMoveStop.y;
	session->isMove = false;

	//SERVER메세지 만들어서 브로드 캐스트
	CProtocol::stPACKET_S_MOVE_STOP serverMoveStop;
	serverMoveStop.id = session->id;
	serverMoveStop.moveDirection = session->moveDirection;
	serverMoveStop.x = session->x;
	serverMoveStop.y = session->y;

	CServer::sendBroadCast(session, (char*)&serverMoveStop, sizeof(serverMoveStop), dfPACKET_SC_MOVE_STOP);
}
void CServer::handle_PACKET_C_MOVE_ATTACK1(stSession* session) {

	//디큐
	CProtocol::stPACKET_C_ATTACK1 clientAttack1;
	session->recvQueue.Dequeue((char*)&clientAttack1, sizeof(clientAttack1));
	cout << "[MESSAE] " << "_C_MOVE_ATTACK1" << " " << "SESSION ID: " << session->id << " / ";
	cout << "Direction: " << (short)clientAttack1.attackDirection << " / " << clientAttack1.x << " " << clientAttack1.y << "\n";

	//이동범위 오류 체크
	bool resultMoveError = CServer::moveError(session, clientAttack1.x, clientAttack1.y);
	if (resultMoveError == true) {
		CServer::disconnectPlayer(session);
		return;
	}
	//세션 업데이트
	session->attackDirection = clientAttack1.attackDirection;
	session->x = clientAttack1.x;
	session->y = clientAttack1.y;
	session->isMove = false;

	//충돌처리 
	CServer::conflictAttack(session, dfATTACK1_RANGE_X, dfATTACK1_RANGE_Y);

	//SERVER메세지 만들어서 브로드 캐스트
	CProtocol::stPACKET_S_ATTACK1 serverAttack1;
	serverAttack1.id = session->id;
	serverAttack1.attackDirection = clientAttack1.attackDirection;
	serverAttack1.x = clientAttack1.x;
	serverAttack1.y = clientAttack1.y;

	CServer::sendBroadCast(session, (char*)&serverAttack1, sizeof(serverAttack1), dfPACKET_SC_ATTACK1);
}
void CServer::handle_PACKET_C_MOVE_ATTACK2(stSession* session) {

	//디큐
	CProtocol::stPACKET_C_ATTACK2 clientAttack2;
	session->recvQueue.Dequeue((char*)&clientAttack2, sizeof(clientAttack2));
	cout << "[MESSAE] " << "_C_MOVE_ATTACK2" << " " << "SESSION ID: " << session->id << " / ";
	cout << "Direction: " << (short)clientAttack2.attackDirection << " / " << clientAttack2.x << " " << clientAttack2.y << "\n";
	//이동범위 확인
	bool resultMoveError = CServer::moveError(session, clientAttack2.x, clientAttack2.y);
	if (resultMoveError == true) {
		CServer::disconnectPlayer(session);
		return;
	}
	//세션 업데이트
	session->attackDirection = clientAttack2.attackDirection;
	session->x = clientAttack2.x;
	session->y = clientAttack2.y;
	session->isMove = false;

	//충돌처리 
	CServer::conflictAttack(session, dfATTACK2_RANGE_X, dfATTACK2_RANGE_Y);

	//SERVER메세지 만들어서 브로드 캐스트
	CProtocol::stPACKET_S_ATTACK2 serverAttack2;
	serverAttack2.id = session->id;
	serverAttack2.attackDirection = session->attackDirection;
	serverAttack2.x = session->x;
	serverAttack2.y = session->y;

	CServer::sendBroadCast(session, (char*)&serverAttack2, sizeof(serverAttack2), dfPACKET_SC_ATTACK2);
}
void CServer::handle_PACKET_C_MOVE_ATTACK3(stSession* session) {

	//디큐
	CProtocol::stPACKET_C_ATTACK3 clientAttack3;
	session->recvQueue.Dequeue((char*)&clientAttack3, sizeof(clientAttack3));

	cout << "[MESSAE] " << "_C_MOVE_ATTACK3" << " " << "SESSION ID: " << session->id << " / ";
	cout << "Direction: " << (short)clientAttack3.attackDirection << " / " << clientAttack3.x << " " << clientAttack3.y << "\n";

	//이동범위 확인하기
	bool resultMoveError = CServer::moveError(session, clientAttack3.x, clientAttack3.y);
	if (resultMoveError == true) {
		CServer::disconnectPlayer(session);
		return;
	}
	//세션 업데이트
	session->attackDirection = clientAttack3.attackDirection;
	session->x = clientAttack3.x;
	session->y = clientAttack3.y;
	session->isMove = false;

	//충돌처리 
	CServer::conflictAttack(session, dfATTACK3_RANGE_X, dfATTACK3_RANGE_Y);

	//SERVER메세지 만들어서 브로드 캐스트
	CProtocol::stPACKET_S_ATTACK3 serverAttack3;
	serverAttack3.id = session->id;
	serverAttack3.attackDirection = session->attackDirection;
	serverAttack3.x = session->x;
	serverAttack3.y = session->y;

	CServer::sendBroadCast(session, (char*)&serverAttack3, sizeof(serverAttack3), dfPACKET_SC_ATTACK3);
}
void CServer::conflictAttack(stSession* session, short attackRangeX, short attackRangeY) {


	int iCount;
	bool resultConflict = false;
	for (iCount = 0; iCount < 63; iCount++) {
		if (CGlobal::sessionArray[iCount].clientSock == INVALID_SOCKET) {
			continue;
		}
		if (session->clientSock == CGlobal::sessionArray[iCount].clientSock) {
			continue;
		}
		//공격 범위 확인하기
		resultConflict = CServer::isConflict(session, &CGlobal::sessionArray[iCount], attackRangeX, attackRangeY);

		if (resultConflict == true) {

			CGlobal::sessionArray[iCount].hp = CGlobal::sessionArray[iCount].hp - 20;
			//패킷 메세지 만들기 
			CProtocol::stPACKET_S_DAMAGE damage;
			damage.attackId = session->id;
			damage.damageId = CGlobal::sessionArray[iCount].id;
			damage.damageHp = CGlobal::sessionArray[iCount].hp;
			CServer::sendBroadCast(nullptr, (char*)&damage, sizeof(damage), dfPACKET_SC_DAMAGE);
			cout << "ATTACK!!" << " " << "TARGET ID:" << damage.damageId << " / " << "TARGET HP: " << (int)damage.damageHp << "\n";
		}
		//여기 굳이 false를 만들 필요가 없지 않나?
	}
}
bool CServer::isConflict(stSession* session, stSession* target, short attackRangeX, short attackRangeY) {


	//y좌표 충돌 범위가 아님 ->이거 변수명으로 빼는게 나을지?
	short attackMinRangeY = session->y - attackRangeY;
	short attackMaxRangeY = session->y + attackRangeY;

	short attackMinRangeX = session->x - attackRangeX;
	short attackMaxRangeX = session->x + attackRangeX;

	cout << "[JUDJE_ATTACK]" << " " << "ATTACK ID: " << session->id << " / " << "TARGET ID: " << target->id << " ";
	cout << "ATTACK DIR: " << (int)session->attackDirection << " / ";
	cout << "ATTACK POS: " << session->x << " " << session->y << "\n";
	cout << "TARGET DIR: " << (int)target->attackDirection << " / ";
	cout << "TARGET POS: " << target->x << " " << target->y << "\n";

	if (target->y<attackMinRangeY || target->y>attackMaxRangeY) {
		return false;
	}
	if (session->attackDirection == dfPACKET_MOVE_DIR_LL) {
		if (target->x <= session->x && target->x > attackMinRangeX) {
			return true;
		}
	}
	else if (session->attackDirection == dfPACKET_MOVE_DIR_RR) {
		if (target->x >= session->x && target->x < attackMaxRangeX) {
			return true;
		}
	}

	return false;
}

bool CServer::moveError(stSession* session, short packetX, short packetY)
{

	if (abs(session->x - packetX) > dfERROR_RANGE || abs(session->y - packetY) > dfERROR_RANGE) {
		return true;
	}
	return false;
}

void CServer::disconnectPlayer(stSession* session) {

	CProtocol::stPACKET_S_DELETE_CHARACTER deleteCharacter;
	deleteCharacter.id = session->id;
	CServer::sendBroadCast(session, (char*)&deleteCharacter, sizeof(deleteCharacter), dfPACKET_SC_DELETE_CHARACTER);
	closesocket(session->clientSock);
	//세션 업데이트 
	session->clientSock = INVALID_SOCKET;
	CGlobal::cnt--;
}
void CServer::sendUniCast(stSession* session, char* message, int messageSize, int type) {

	//패킷 헤더 만들기 
	CProtocol::stPacketHeader packetHeader;
	packetHeader.byCode = 0x89;
	packetHeader.bySize = messageSize;
	packetHeader.byType = type;

	session->sendQueue.Enqueue((char*)&packetHeader, 3);
	session->sendQueue.Enqueue((char*)message, messageSize);
}
void CServer::sendBroadCast(stSession* session, char* message, int messageSize, int type) {
	//패킷 헤더 만들기 
	CProtocol::stPacketHeader packetHeader;
	packetHeader.byCode = 0x89;
	packetHeader.bySize = messageSize;
	packetHeader.byType = type;

	int iCount;
	for (iCount = 0; iCount < 63; iCount++) {
		if (CGlobal::sessionArray[iCount].clientSock == INVALID_SOCKET) {
			continue;
		}
		if (session == &CGlobal::sessionArray[iCount]) {
			continue;
		}
		CGlobal::sessionArray[iCount].sendQueue.Enqueue((char*)&packetHeader, 3); //헤더
		CGlobal::sessionArray[iCount].sendQueue.Enqueue(message, messageSize); //메세지
	}
}
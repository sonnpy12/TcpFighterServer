#include "stdafx.h"
#include "CLogic.h"
#include "CGlobal.h"
#include "CServer.h"
#include "PacketDefine.h"
void CLogic::update()
{
	//로직 업데이트 
	//HP 0인 경우 disconnectPlayer하기
	//direction방향으로 캐릭터 이동시키기 
	int iCount;
	for (iCount = 0; iCount < 63; iCount++) {
		if (CGlobal::sessionArray[iCount].clientSock == INVALID_SOCKET) {
			continue;
		}
		if (CGlobal::sessionArray[iCount].hp <= 0) {
			CServer::disconnectPlayer(&CGlobal::sessionArray[iCount]);
			continue;
		}
		CLogic::moveCharacter(&CGlobal::sessionArray[iCount]);

	}

}
void CLogic::moveCharacter(stSession* session) {

	if (session->isMove == false) {
		return;
	}
	int direct[8][2] = {
		{0,-3}, //0		LL
		{-2,-3}, //1	LU
		{-2,0}, //2		UU
		{-2,3}, //3		RU
		{0,3}, //4		RR			
		{2,3}, //5		RD
		{2,0}, //6		DD
		{2,-3},//7		LD	
	};

	int dx = session->x + direct[session->moveDirection][1];
	int dy = session->y + direct[session->moveDirection][0];

	//화면 이동 영역 ->움직일 때만 

	if (dy< dfRANGE_MOVE_TOP || dy>dfRANGE_MOVE_BOTTOM || dx< dfRANGE_MOVE_LEFT || dx>dfRANGE_MOVE_RIGHT) {

		return;
	}
	session->x = dx;
	session->y = dy;
	cout << "MoveCharacter:" << dx << " " << dy << "\n";
}
#pragma once
#pragma pack(1)
//패킷 헤더 만들기 

class CProtocol {

public:
	struct stPacketHeader {
		BYTE byCode;
		BYTE bySize;
		BYTE byType;
	};
	//내 캐릭터 패킷 메세지
	struct stPACKET_S_CREATE_MY_CHARACTER {
		int id;
		char moveDirection;
		short x;
		short y;
		char hp;
	};
	//다른 캐릭터 패킷 메세지 
	struct stPACKET_S_CHREATE_OTHER_CHARACTER {
		int id;
		char moveDirection;
		short x;
		short y;
		char hp;
	};
	//캐릭터 삭제 메세지
	struct stPACKET_S_DELETE_CHARACTER {
		int id;
	};
	//캐릭터 이동시작 메세지 client->server
	struct stPACKET_C_MOVE_START {
		char moveDirection;
		short x;
		short y;
	};
	//캐릭터 이동시작 메세지 server->client
	struct stPACKET_S_MOVE_START {
		int id;
		char moveDirection;
		short x;
		short y;
	};
	//캐릭터 이동중지 메세지 client->server
	struct stPACKET_C_MOVE_STOP {
		char moveDirection;
		short x;
		short y;
	};
	//캐릭터 이동중지 메세지 server->client
	struct stPACKET_S_MOVE_STOP {
		int id;
		char moveDirection;
		short x;
		short y;
	};
	//캐릭터 공격 메세지 attack1 client->server
	struct stPACKET_C_ATTACK1 {
		char attackDirection;
		short x;
		short y;
	};
	//캐릭터 공격 메세지 attack1 server->client
	struct stPACKET_S_ATTACK1 {
		int id;
		char attackDirection;
		short x;
		short y;
	};
	//캐릭터 공격 메세지 attack2 client->server
	struct stPACKET_C_ATTACK2 {
		char attackDirection;
		short x;
		short y;
	};
	//캐릭터 공격 메세지 attack2 server->client
	struct stPACKET_S_ATTACK2 {
		int id;
		char attackDirection;
		short x;
		short y;
	};
	//캐릭터 공격 메세지 attack3 client->server
	struct stPACKET_C_ATTACK3 {
		char attackDirection;
		short x;
		short y;
	};
	//캐릭터 공격 메세지 attack3 server->client
	struct stPACKET_S_ATTACK3 {
		int id;
		char attackDirection;
		short x;
		short y;
	};
	//캐릭터 데미지 메세지 server->client
	struct stPACKET_S_DAMAGE {
		int attackId;
		int damageId;
		char damageHp;
	};
#pragma pack()
};
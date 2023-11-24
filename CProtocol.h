#pragma once
#pragma pack(1)
//��Ŷ ��� ����� 

class CProtocol {

public:
	struct stPacketHeader {
		BYTE byCode;
		BYTE bySize;
		BYTE byType;
	};
	//�� ĳ���� ��Ŷ �޼���
	struct stPACKET_S_CREATE_MY_CHARACTER {
		int id;
		char moveDirection;
		short x;
		short y;
		char hp;
	};
	//�ٸ� ĳ���� ��Ŷ �޼��� 
	struct stPACKET_S_CHREATE_OTHER_CHARACTER {
		int id;
		char moveDirection;
		short x;
		short y;
		char hp;
	};
	//ĳ���� ���� �޼���
	struct stPACKET_S_DELETE_CHARACTER {
		int id;
	};
	//ĳ���� �̵����� �޼��� client->server
	struct stPACKET_C_MOVE_START {
		char moveDirection;
		short x;
		short y;
	};
	//ĳ���� �̵����� �޼��� server->client
	struct stPACKET_S_MOVE_START {
		int id;
		char moveDirection;
		short x;
		short y;
	};
	//ĳ���� �̵����� �޼��� client->server
	struct stPACKET_C_MOVE_STOP {
		char moveDirection;
		short x;
		short y;
	};
	//ĳ���� �̵����� �޼��� server->client
	struct stPACKET_S_MOVE_STOP {
		int id;
		char moveDirection;
		short x;
		short y;
	};
	//ĳ���� ���� �޼��� attack1 client->server
	struct stPACKET_C_ATTACK1 {
		char attackDirection;
		short x;
		short y;
	};
	//ĳ���� ���� �޼��� attack1 server->client
	struct stPACKET_S_ATTACK1 {
		int id;
		char attackDirection;
		short x;
		short y;
	};
	//ĳ���� ���� �޼��� attack2 client->server
	struct stPACKET_C_ATTACK2 {
		char attackDirection;
		short x;
		short y;
	};
	//ĳ���� ���� �޼��� attack2 server->client
	struct stPACKET_S_ATTACK2 {
		int id;
		char attackDirection;
		short x;
		short y;
	};
	//ĳ���� ���� �޼��� attack3 client->server
	struct stPACKET_C_ATTACK3 {
		char attackDirection;
		short x;
		short y;
	};
	//ĳ���� ���� �޼��� attack3 server->client
	struct stPACKET_S_ATTACK3 {
		int id;
		char attackDirection;
		short x;
		short y;
	};
	//ĳ���� ������ �޼��� server->client
	struct stPACKET_S_DAMAGE {
		int attackId;
		int damageId;
		char damageHp;
	};
#pragma pack()
};
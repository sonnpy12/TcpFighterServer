#pragma once
#pragma once
#include "Session.h"

class CGlobal {
public:
	inline static stSession		sessionArray[63];
	inline static int			id;					//ID�Ҵ� 
	inline static int			cnt;				//���� FD_SET����ü ��á���� ��á���� Ȯ���ϱ�
	inline static SOCKET		clientSock;			//��ε�ĳ���� session
	inline static SOCKADDR_IN	clientAddr;			//���� ���� readset������ �ְ� backlogqueue���� ������ Ŭ���̾�Ʈ �ּ�
	inline static int			recvCount;
};



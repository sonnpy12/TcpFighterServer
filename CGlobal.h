#pragma once
#pragma once
#include "Session.h"

class CGlobal {
public:
	inline static stSession		sessionArray[63];
	inline static int			id;					//ID할당 
	inline static int			cnt;				//지금 FD_SET구조체 꽉찼는지 안찼는지 확인하기
	inline static SOCKET		clientSock;			//브로드캐스팅 session
	inline static SOCKADDR_IN	clientAddr;			//리슨 소켓 readset반응이 있고 backlogqueue에서 연결할 클라이언트 주소
	inline static int			recvCount;
};



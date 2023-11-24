#pragma once

class CRingBuffer {


public:

	CRingBuffer(int iBufferSize);
	CRingBuffer();
	int GetBufferSize();
	int GetUseSize(); //현재 사용중인 용량 얻기
	int GetFreeSize(); //현재 버퍼에 남은 용량 얻기
	int Enqueue(char* chpData, int iSize);
	int Dequeue(char* chpDest, int iSize);
	int Peek(char* chpDest, int iSize);
	void ClearBuffer();
	int DirectEnqueueSize();
	int DirectDequeueSize();
	int MoveFront(int iSize);
private:

	int		_front;
	int		_rear;

	int		_totalSize;
	int		_leftSize;
	char* _ringBuffer;

};
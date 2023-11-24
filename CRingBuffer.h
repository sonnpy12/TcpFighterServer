#pragma once

class CRingBuffer {


public:

	CRingBuffer(int iBufferSize);
	CRingBuffer();
	int GetBufferSize();
	int GetUseSize(); //���� ������� �뷮 ���
	int GetFreeSize(); //���� ���ۿ� ���� �뷮 ���
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
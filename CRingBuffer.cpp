#include "stdafx.h"
#include "CRingBuffer.h"



CRingBuffer::CRingBuffer() {

	_front = 0;
	_rear = 0;
	_totalSize = 10000;
	_ringBuffer = new char[_totalSize];

}
CRingBuffer::CRingBuffer(int iBufferSize) {

	_front = 0;
	_rear = 0;
	_totalSize = iBufferSize;
	_ringBuffer = new char[_totalSize];
}
int CRingBuffer::GetBufferSize() {

	return _totalSize;

}
int CRingBuffer::GetUseSize() {

	int useSize;
	//큐가 꽉차 있는 상황 
	if (_front == (_rear + 1) % _totalSize) {
		useSize = _totalSize - 1;
	}
	else if (_rear < _front) {
		useSize = _totalSize - (_front - _rear);
	}
	else if (_rear > _front) {
		useSize = _rear - _front;
	}

	else if (_front == _rear) {
		useSize = 0;
	}

	return useSize;

}
int CRingBuffer::GetFreeSize() {

	int freeSize;
	//큐가 꽉차 있을 때 
	if (_front == (_rear + 1) % _totalSize) {
		freeSize = 0;
	}
	else if (_rear < _front) {
		freeSize = _front - _rear;
	}
	else if (_rear > _front) {
		freeSize = _totalSize - (_rear - _front) - 1;
	}
	//큐가 비어 있는 상황
	else if (_rear == _front) {
		freeSize = 9999;
	}
	return freeSize;

}
int CRingBuffer::Enqueue(char* chpData, int iSize) {


	if (GetFreeSize() < iSize) {
		return 0;
	}
	if (_totalSize - _rear < iSize) {
		int left = _totalSize - _rear;
		memcpy(_ringBuffer + _rear, chpData, left);
		memcpy(_ringBuffer + ((_rear + left) % _totalSize), chpData + left, iSize - left);
	}
	else { memcpy(_ringBuffer + _rear, chpData, iSize); }
	_rear = (_rear + iSize) % _totalSize;

	return iSize;
}
int CRingBuffer::Dequeue(char* chpData, int iSize) {

	if (GetUseSize() < iSize || GetUseSize() == 0) {
		return 0;
	}
	if (_totalSize - _front < iSize) {
		int left = _totalSize - _front;
		memcpy(chpData, _ringBuffer + _front, left);
		//memset(_ringBuffer + _front, 0, left);
		memcpy(chpData + left, _ringBuffer + ((_front + left) % _totalSize), iSize - left);
		//memset(_ringBuffer + (_front + left) % _totalSize, 0, iSize - left);
	}
	else { memcpy(chpData, _ringBuffer + _front, iSize); }
	_front = (_front + iSize) % _totalSize;
	return iSize;
}
int CRingBuffer::Peek(char* chpDest, int iSize) {

	if (GetUseSize() < iSize || GetUseSize() == 0) {
		return 0;
	}
	if (_totalSize - _front < iSize) {
		int left = _totalSize - _front;
		memcpy(chpDest, _ringBuffer + _front, left);
		memcpy(chpDest + left, _ringBuffer + ((_front + left) % _totalSize), iSize - left);
	}
	else { memcpy(chpDest, _ringBuffer + _front, iSize); }
	return iSize;
}
void CRingBuffer::ClearBuffer() {

	_rear = 0;
	_front = 0;
}
int CRingBuffer::DirectEnqueueSize() {

	int DirectEnSize;

	if (_rear >= _front) {
		DirectEnSize = _totalSize - _rear;
	}
	else if (_rear < _front) {
		DirectEnSize = _front - _rear;
	}
	return DirectEnSize;
}
int CRingBuffer::DirectDequeueSize() {

	int DirectDeSize;
	if (_front > _rear) {

		DirectDeSize = _totalSize - _front;
	}
	else if (_front < _rear) {
		DirectDeSize = _rear - _front;
	}
	else if (_front == _rear) {
		DirectDeSize = 0;
	}
	return DirectDeSize;
}
int CRingBuffer::MoveFront(int iSize) {

	_front = (_front + iSize) % _totalSize;
	return iSize;

}
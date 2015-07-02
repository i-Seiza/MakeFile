#include "StdAfx.h"
#include "WriteFileBeginAll.h"

CWriteFileBeginAll::CWriteFileBeginAll(void)
{
}

CWriteFileBeginAll::CWriteFileBeginAll(LONGLONG _bufSize, LONGLONG _QuadPart)
{
	QuadPart = _QuadPart;
	bufSize = _bufSize;
}

CWriteFileBeginAll::~CWriteFileBeginAll(void)
{
}

// �ŏ������C�ɂ��ׂĂ̏�����������
bool CWriteFileBeginAll::Execute(HANDLE *hFile, CDiskVolume v)
{
	buf = (TCHAR*)VirtualAlloc(NULL, bufSize, MEM_COMMIT, PAGE_READWRITE);
	bool bSuccess = SetFilePointer(hFile);
	bSuccess = WriteFile( hFile );
	VirtualFree(buf, 0, MEM_FREE);
	return bSuccess;
}

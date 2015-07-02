#include "StdAfx.h"
#include "WriteFileBack512.h"

CWriteFileBack512::CWriteFileBack512(void)
{
}

CWriteFileBack512::CWriteFileBack512(LONGLONG _bufSize, LONGLONG _QuadPart)
{
	QuadPart = _QuadPart;
	bufSize = _bufSize;
}

CWriteFileBack512::~CWriteFileBack512(void)
{
}

// ç≈å„Ç…512ÉoÉCÉgï™ÇæÇØèëÇ´çûÇﬁ
bool CWriteFileBack512::Execute(HANDLE *hFile, CDiskVolume v)
{
	buf = (TCHAR*)VirtualAlloc(NULL, bufSize, MEM_COMMIT, PAGE_READWRITE);

	bool bSuccess = SetFilePointer(hFile);

	bSuccess = WriteFile( hFile );

	VirtualFree(buf, 0, MEM_FREE);

	return bSuccess;
}

#include "StdAfx.h"
#include "WriteFilePart512.h"

CWriteFilePart512::CWriteFilePart512(void)
{
}

CWriteFilePart512::CWriteFilePart512(LONGLONG _bufSize, LONGLONG _QuadPart)
{
	QuadPart = _QuadPart;
	bufSize = _bufSize;
}

CWriteFilePart512::~CWriteFilePart512(void)
{
}

bool CWriteFilePart512::Execute(HANDLE *hFile, CDiskVolume v)
{
	CLog log;
	buf = (TCHAR*)VirtualAlloc(NULL, bufSize, MEM_COMMIT, PAGE_READWRITE);

	bool bSuccess = SetFilePointer(hFile);


	for ( DWORD i = 0; i < v.GetNumberOfFreeClusters(); i++ )
	{
		for ( DWORD j = 0; j < v.GetSectorsPerCluster(); j++)
		{

			DWORD size = 0;
			bool bSuccess = ::WriteFile( *hFile, buf, bufSize, &size, NULL );

			if(bSuccess == 0)	break;

		}
	}

	log.AddResult(L"ƒtƒ@ƒCƒ‹‚Ì‘‚«ž‚Ý", bSuccess != 0 );
	VirtualFree(buf, 0, MEM_FREE);

	return bSuccess;
}

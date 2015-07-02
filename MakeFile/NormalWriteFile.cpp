#include "StdAfx.h"
#include "NormalWriteFile.h"

CNormalWriteFile::CNormalWriteFile(void)
{
	buf = NULL;
}

CNormalWriteFile::CNormalWriteFile(LONGLONG _bufSize, LONGLONG _QuadPart)
{
	QuadPart = _QuadPart;
	bufSize = _bufSize;
}

CNormalWriteFile::~CNormalWriteFile(void)
{
}

bool CNormalWriteFile::Execute(HANDLE *hFile, CDiskVolume v)
{
	return false;
}

bool CNormalWriteFile::SetFilePointer(HANDLE *hFile)
{
	CLog log;
	LARGE_INTEGER li, NewFilePointer;
	li.QuadPart = QuadPart;
	log.Add(L"\tli.QuadPart", li.QuadPart);

	DWORD dwMoveMethod = li.QuadPart >= 0 ? FILE_BEGIN : FILE_END;
	bool bSuccess = SetFilePointerEx(
		*hFile,
		li,
		&NewFilePointer,
		dwMoveMethod
		);
	log.AddResult(L"SetFilePointerEx", bSuccess != 0);

	return bSuccess;

}

bool CNormalWriteFile::WriteFile(HANDLE *hFile)
{
	CLog log;
	DWORD size = 0;
	bool bSuccess = ::WriteFile( *hFile, buf, bufSize, &size, NULL );
	log.AddResult(L"ÉtÉ@ÉCÉãÇÃèëÇ´çûÇ›", bSuccess != 0 );

	return bSuccess;
}

#include "StdAfx.h"
#include "FileIO.h"
#include "Log.h"

CFileIO::CFileIO(void)
{
}

CFileIO::~CFileIO(void)
{
}

// ファイルを開く
bool CFileIO::FileOpen(HANDLE *hFile, std::wstring path, DWORD dwFlagsAndAttributes)
{
	CLog log;

	// 一旦ファイルをopen
	*hFile = CreateFile(
		path.c_str(),
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		dwFlagsAndAttributes,
		NULL
		);
	if (*hFile == INVALID_HANDLE_VALUE)
	{
		// ファイルが開けなかった。どうしよう？
		log.AddResult(L"ファイルオープン", false);
		return false;
	}

	return true;
}

bool CFileIO::FileClose(HANDLE *hFile)
{
	if (*hFile)
	{
		CloseHandle(*hFile);
	}
	return true;
}

bool CFileIO::FixFileSize( HANDLE *hFile, LONGLONG pointer, LONGLONG size )
{
	CLog log;

	SetFilePointer( hFile, pointer );

	bool bSuccess = SetEndOfFile(*hFile);
	log.AddResult(L"SetEndOfFile", bSuccess != 0);
	if(bSuccess == 0)	return false;

	return true;
}

// ファイルサイズを確定する
bool CFileIO::SetFilePointer( HANDLE *hFile, LONGLONG pointer )
{
	CLog log;

	LARGE_INTEGER li;
	li.QuadPart = pointer;

	LARGE_INTEGER NewFilePointer;
	bool bSuccess = SetFilePointerEx(
		*hFile,
		li,
		&NewFilePointer,
		FILE_BEGIN
		);
	log.AddResult(L"SetFilePointerEx(1)", bSuccess != 0);
	if(bSuccess == 0)	return false;

	return true;
}
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
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
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

// ファイルサイズを確定する
bool CFileIO::SetFileSize( HANDLE *hFile, LONGLONG size )
{
	CLog log;

	LARGE_INTEGER li;
	li.QuadPart = size;


	LARGE_INTEGER NewFilePointer;
	bool bSuccess = SetFilePointerEx(
		*hFile,
		li,
		&NewFilePointer,
		FILE_BEGIN
		);
	log.AddResult(L"SetFilePointerEx(1)", bSuccess != 0);
	if(bSuccess == 0)	return false;

	bSuccess = SetEndOfFile(*hFile);
	log.AddResult(L"SetEndOfFile", bSuccess != 0);
	if(bSuccess == 0)	return false;

	return true;
}
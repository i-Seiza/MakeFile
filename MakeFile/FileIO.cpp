#include "StdAfx.h"
#include "FileIO.h"
#include "Log.h"

CFileIO::CFileIO(void)
{
}

CFileIO::~CFileIO(void)
{
}

// �t�@�C�����J��
bool CFileIO::FileOpen(HANDLE *hFile, TCHAR *path, DWORD dwFlagsAndAttributes)
{
	CLog log;

	// ��U�t�@�C����open
	*hFile = CreateFile(
		path,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		dwFlagsAndAttributes,
		NULL
		);
	if (*hFile == INVALID_HANDLE_VALUE)
	{
		// �t�@�C�����J���Ȃ������B�ǂ����悤�H
		log.AddResult(L"�t�@�C���I�[�v��", false);
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
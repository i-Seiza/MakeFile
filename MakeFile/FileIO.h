#pragma once
#include "Windows.h"
#include <string>



class CFileIO
{
public:
	CFileIO(void);
	~CFileIO(void);


	bool FileOpen( HANDLE *hFile, std::wstring path, DWORD dwFlagsAndAttributes );
	bool FileClose(HANDLE *hFile);
	bool SetFileSize( HANDLE *hFile, LONGLONG size );

};

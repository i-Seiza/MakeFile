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
	bool SetFilePointer( HANDLE *hFile, LONGLONG pointer );
	bool FixFileSize( HANDLE *hFile, LONGLONG pointer, LONGLONG size );

};

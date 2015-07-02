#pragma once
#include "Log.h"
#include "DiskVolume.h"



class CNormalWriteFile
{
public:
	CNormalWriteFile(void);
	CNormalWriteFile(LONGLONG bufSize, LONGLONG QuadPart);
	~CNormalWriteFile(void);

	virtual bool Execute(HANDLE *hFile, CDiskVolume v);

protected:
	bool SetFilePointer(HANDLE *hFile);
	bool WriteFile(HANDLE *hFile);

	TCHAR* buf;
	LONGLONG QuadPart;
	LONGLONG bufSize;
};

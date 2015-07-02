#pragma once
#include "normalwritefile.h"

class CWriteFileBeginAll :
	public CNormalWriteFile
{
public:
	CWriteFileBeginAll(void);
	CWriteFileBeginAll(LONGLONG bufSize, LONGLONG QuadPart);
	~CWriteFileBeginAll(void);

	bool Execute(HANDLE *hFile, CDiskVolume v);

};

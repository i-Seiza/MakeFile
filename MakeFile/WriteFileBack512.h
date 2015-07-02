#pragma once
#include "normalwritefile.h"

class CWriteFileBack512 :
	public CNormalWriteFile
{
public:
	CWriteFileBack512(void);
	CWriteFileBack512(LONGLONG bufSize, LONGLONG QuadPart);
	~CWriteFileBack512(void);

	bool Execute(HANDLE *hFile, CDiskVolume v);
};

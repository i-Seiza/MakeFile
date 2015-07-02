#pragma once
#include "normalwritefile.h"

class CWriteFilePart512 :
	public CNormalWriteFile
{
public:
	CWriteFilePart512(void);
	CWriteFilePart512(LONGLONG bufSize, LONGLONG QuadPart);
	~CWriteFilePart512(void);

	bool Execute(HANDLE *hFile, CDiskVolume v);

};

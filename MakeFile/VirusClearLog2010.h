#pragma once

#include "virusclearlog.h"


// �E�C���X�N���A���O��ǂݎ��
class CVirusClearLog2010 :
	public CVirusClearLog
{
public:
	CVirusClearLog2010(void);
	~CVirusClearLog2010(void);

public:
	void Read( const TCHAR *path );
};

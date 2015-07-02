// MakeFile.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "windows.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "DiskVolume.h"
#include "Log.h"
#include <locale.h>

#include "WriteFileBack512.h"
#include "WriteFileBeginAll.h"
#include "WriteFilePart512.h"
#include "FileIO.h"



const TCHAR pDummyFile[] = L"E:\\dummy.txt";


// �t�@�C���T�C�Y���m�肷��
bool SetFileSize( HANDLE *hFile, LONGLONG size )
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


// Sparse File�ɂ�鏑������
bool SparseFile( HANDLE *hFile, CDiskVolume v )
{
	CLog log;
	if ( !v.IsSupportsSparseFiles() )
	{
		log.Add(L"SparseFile��Ή�");
		return false;
	}

	log.Add(L"SparseFile�Ή�");


	// ��U�t�@�C����open
	CFileIO io;
	bool bSuccess = io.FileOpen( hFile, v.GetPath(), FILE_ATTRIBUTE_NORMAL );
	if (!bSuccess)
	{
		return false;
	}

	//SparseFile�ł���Ɩ��L
	DWORD dw;
	DeviceIoControl(*hFile, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &dw, NULL);

	// �t�@�C������������
	LONG lDist = 4095;
	SetFilePointer(*hFile, 0, &lDist, FILE_BEGIN);
	SetEndOfFile(*hFile);

	return bSuccess;
}

// �Ō�512byte�ɏ�������
bool TryEndWrite512ByteNormal(TCHAR *path, DWORD dwFlagsAndAttributes)
{
	DWORD start = GetTickCount();

	// �f�B�X�N�e�ʂ��擾����
	CDiskVolume v;
	bool bSuccess = v.GetDiskVolumeInfo(path);
	if (!bSuccess)
	{
		return false;
	}


	CLog log;
	CFileIO io;

	// �f�B�X�N�󂫗e�ʕ��̃t�@�C�����쐬����
	HANDLE hFile = NULL;
	bSuccess = io.FileOpen( &hFile, v.GetPath(), dwFlagsAndAttributes );
	if (!bSuccess)
	{
		return false;
	}

	// �󂫗e�ʂ��擾
	LONGLONG lFileSize = v.GetTotalNumberOfFreeBytes();

	// �t�@�C���T�C�Y���m�肷��
	bSuccess = SetFileSize( &hFile, lFileSize );
	log.AddResult(L"�t�@�C���T�C�Y���m��", bSuccess);
	log.Add(L"\t�t�@�C���T�C�Y", lFileSize);


	// �Z�N�^�{�����[�����擾
	const int bufSize = v.GetBytesPerSector();


	CWriteFileBack512 wf( bufSize, -1 * bufSize );


	bSuccess = wf.Execute(&hFile, v);

	io.FileClose(&hFile);

	DWORD end = GetTickCount();
	log.Add(L"��������/�b", (end-start)/1000);

	return true;
}

// �擪����512�o�C�g���Ƃɏ�������
bool TryBeginWrite512ByteNormal(TCHAR *path, DWORD dwFlagsAndAttributes, int iTimes)
{
	DWORD start = GetTickCount();

	// �f�B�X�N�e�ʂ��擾����
	CDiskVolume v;
	bool bSuccess = v.GetDiskVolumeInfo(path);
	if (!bSuccess)
	{
		return false;
	}


	CLog log;
	CFileIO io;

	// �f�B�X�N�󂫗e�ʕ��̃t�@�C�����쐬����
	HANDLE hFile = NULL;
	bSuccess = io.FileOpen( &hFile, v.GetPath(), dwFlagsAndAttributes );
	if (!bSuccess)
	{
		return false;
	}

	// �󂫗e�ʂ��擾
	LONGLONG lFileSize = v.GetTotalNumberOfFreeBytes();

	// �t�@�C���T�C�Y���m�肷��
	bSuccess = SetFileSize( &hFile, lFileSize );
	log.AddResult(L"�t�@�C���T�C�Y���m��", bSuccess);
	log.Add(L"\t�t�@�C���T�C�Y", lFileSize);


	// �Z�N�^�{�����[�����擾
	const int bufSize = v.GetBytesPerSector() * iTimes;


	CWriteFilePart512 wf( bufSize, 0 );
	bSuccess = wf.Execute(&hFile, v);

	io.FileClose(&hFile);

	DWORD end = GetTickCount();
	log.Add(L"��������/�b", (end-start)/1000);

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T(""));
	TCHAR *path =  argc > 1 ? argv[1] : NULL ;

	::MessageBox( NULL, L"�A�^�b�`����Ȃ炱���I", L"", MB_OK);


	CLog log;

#if 0
	// �Ō�512�o�C�g���Ƃɏ�������
	TryEndWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL);
	log.Add(L"");
	log.Add(L"");
	TryEndWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH);
#endif

#if 1
	// �擪����512�o�C�g���Ƃɏ�������
	TryBeginWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL, 1);
	log.Add(L"");
	log.Add(L"");
	TryBeginWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, 1);
#endif

#if 0

	TryBeginWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL, 1000*1000);
	log.Add(L"");
	log.Add(L"");
	TryBeginWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, 1000*1000);
#endif

	::MessageBox( NULL, L"���ʊm�F", L"", MB_OK );
	
	return 0;
}


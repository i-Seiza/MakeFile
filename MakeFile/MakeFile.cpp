// MakeFile.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Log.h"
#include "DiskVolume.h"
#include "FileIO.h"
#include <sstream>
#include <iostream>


bool WriteFileObj(HANDLE *hFile, TCHAR* buf, LONGLONG bufSize, CDiskVolume v )
{
	// ��������
	for ( DWORD i = 0; i < v.GetNumberOfFreeClusters(); i++ )
	{
		for ( DWORD j = 0; j < v.GetSectorsPerCluster(); j++)
		{
			DWORD size = 0;
			bool bSuccess = ::WriteFile( *hFile, buf, bufSize, &size, NULL );

			if(bSuccess == false)
			{
				return false;
			}
		}
	}

	return true;

}

bool DeviceIoControlObj( HANDLE *hFile, DWORD dwIoControlCode )
{
	VOLUME_DISK_EXTENTS diskExtents={0};
	DWORD BytesReturned;

	bool bSuccess = DeviceIoControl( *hFile,                     // handle to device
		dwIoControlCode, // dwIoControlCode
		NULL,                                 // lpInBuffer
		0,                                    // nInBufferSize
		&diskExtents,                 // output buffer
		sizeof(diskExtents),               // size of output buffer
		&BytesReturned,            // number of bytes returned
		NULL           // OVERLAPPED structure
		);
	return bSuccess;
}

// �������s
bool Process(HANDLE *hFile, CDiskVolume v)
{
	CLog log;
	bool bSuccess = false;

	// Dismount the volume.
	bSuccess = DeviceIoControlObj( hFile, (DWORD) FSCTL_DISMOUNT_VOLUME);
	if(bSuccess==0)
	{
		log.AddResult(L"DeviceIoControl(FSCTL_DISMOUNT_VOLUME)", bSuccess != 0);
		return false;
	}

	// �擪�Ƀ|�C���^�����킹��
	CFileIO io;
	bSuccess = io.SetFilePointer( hFile, 0 );


	// �������擾
	LONGLONG bufSize = v.GetBytesPerSector() * 1000 * 1000;
	TCHAR* buf = (TCHAR*)VirtualAlloc(NULL, bufSize, MEM_COMMIT, PAGE_READWRITE);

	bSuccess = WriteFileObj( hFile, buf, bufSize, v );
	log.AddResult(L"�t�@�C���̏�������", bSuccess != 0 );

	// ���������
	VirtualFree(buf, 0, MEM_FREE);

	return bSuccess;
}


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T(""));
	::MessageBox( NULL, L"�A�^�b�`����Ȃ炱���I", L"", MB_OK);

	DWORD start = GetTickCount();

	// ���݂̃f�B�X�N�����擾
	CDiskVolume v;
	v.GetDiskVolumeInfo(argc > 1 ? argv[1] : NULL);

	HANDLE hFile = NULL;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;

	CLog log;
	bool bSuccess = false;

	CFileIO io;

	// �w��h���C�u���̒���
	std::wstring drive = std::wstring(L"\\\\.\\") + std::wstring(v.GetDrive());
	if (drive.substr(drive.length()-1) == L"\\")
	{
		drive.erase( --drive.end() );	// ������\\�����Ă���̂ŁA������폜����
	}

	// �t�@�C�����J��
	io.FileOpen( &hFile, drive, dwFlagsAndAttributes );

	// ���������s����
	bSuccess = Process(&hFile, v);

	// �t�@�C�������
	io.FileClose( &hFile );

	DWORD end = GetTickCount();
	log.Add(L"��������/�b", (end-start)/1000);

	::MessageBox( NULL, L"���ʊm�F", L"", MB_OK );

	return 0;
}


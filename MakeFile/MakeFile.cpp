// MakeFile.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Log.h"
#include "DiskVolume.h"
#include "FileIO.h"
#include <sstream>
#include <iostream>


bool WriteFileObj(HANDLE *hFile, TCHAR* buf, LONGLONG bufSize, CDiskVolume v )
{
	// 書き込む
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

// 処理実行
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

	// 先頭にポインタを合わせる
	CFileIO io;
	bSuccess = io.SetFilePointer( hFile, 0 );


	// メモリ取得
	LONGLONG bufSize = v.GetBytesPerSector() * 1000 * 1000;
	TCHAR* buf = (TCHAR*)VirtualAlloc(NULL, bufSize, MEM_COMMIT, PAGE_READWRITE);

	bSuccess = WriteFileObj( hFile, buf, bufSize, v );
	log.AddResult(L"ファイルの書き込み", bSuccess != 0 );

	// メモリ解放
	VirtualFree(buf, 0, MEM_FREE);

	return bSuccess;
}


int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T(""));
	::MessageBox( NULL, L"アタッチするならここ！", L"", MB_OK);

	DWORD start = GetTickCount();

	// 現在のディスク情報を取得
	CDiskVolume v;
	v.GetDiskVolumeInfo(argc > 1 ? argv[1] : NULL);

	HANDLE hFile = NULL;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;

	CLog log;
	bool bSuccess = false;

	CFileIO io;

	// 指定ドライブ名の調整
	std::wstring drive = std::wstring(L"\\\\.\\") + std::wstring(v.GetDrive());
	if (drive.substr(drive.length()-1) == L"\\")
	{
		drive.erase( --drive.end() );	// 末尾に\\をつけているので、それを削除する
	}

	// ファイルを開く
	io.FileOpen( &hFile, drive, dwFlagsAndAttributes );

	// 処理を実行する
	bSuccess = Process(&hFile, v);

	// ファイルを閉じる
	io.FileClose( &hFile );

	DWORD end = GetTickCount();
	log.Add(L"処理時間/秒", (end-start)/1000);

	::MessageBox( NULL, L"結果確認", L"", MB_OK );

	return 0;
}


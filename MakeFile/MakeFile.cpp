// MakeFile.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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


// ファイルサイズを確定する
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


// Sparse Fileによる書き込み
bool SparseFile( HANDLE *hFile, CDiskVolume v )
{
	CLog log;
	if ( !v.IsSupportsSparseFiles() )
	{
		log.Add(L"SparseFile非対応");
		return false;
	}

	log.Add(L"SparseFile対応");


	// 一旦ファイルをopen
	CFileIO io;
	bool bSuccess = io.FileOpen( hFile, v.GetPath(), FILE_ATTRIBUTE_NORMAL );
	if (!bSuccess)
	{
		return false;
	}

	//SparseFileであると明記
	DWORD dw;
	DeviceIoControl(*hFile, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &dw, NULL);

	// ファイルを書き込む
	LONG lDist = 4095;
	SetFilePointer(*hFile, 0, &lDist, FILE_BEGIN);
	SetEndOfFile(*hFile);

	return bSuccess;
}

// 最後512byteに書き込む
bool TryEndWrite512ByteNormal(TCHAR *path, DWORD dwFlagsAndAttributes)
{
	DWORD start = GetTickCount();

	// ディスク容量を取得する
	CDiskVolume v;
	bool bSuccess = v.GetDiskVolumeInfo(path);
	if (!bSuccess)
	{
		return false;
	}


	CLog log;
	CFileIO io;

	// ディスク空き容量分のファイルを作成する
	HANDLE hFile = NULL;
	bSuccess = io.FileOpen( &hFile, v.GetPath(), dwFlagsAndAttributes );
	if (!bSuccess)
	{
		return false;
	}

	// 空き容量を取得
	LONGLONG lFileSize = v.GetTotalNumberOfFreeBytes();

	// ファイルサイズを確定する
	bSuccess = SetFileSize( &hFile, lFileSize );
	log.AddResult(L"ファイルサイズを確定", bSuccess);
	log.Add(L"\tファイルサイズ", lFileSize);


	// セクタボリュームを取得
	const int bufSize = v.GetBytesPerSector();


	CWriteFileBack512 wf( bufSize, -1 * bufSize );


	bSuccess = wf.Execute(&hFile, v);

	io.FileClose(&hFile);

	DWORD end = GetTickCount();
	log.Add(L"処理時間/秒", (end-start)/1000);

	return true;
}

// 先頭から512バイトごとに書き込む
bool TryBeginWrite512ByteNormal(TCHAR *path, DWORD dwFlagsAndAttributes, int iTimes)
{
	DWORD start = GetTickCount();

	// ディスク容量を取得する
	CDiskVolume v;
	bool bSuccess = v.GetDiskVolumeInfo(path);
	if (!bSuccess)
	{
		return false;
	}


	CLog log;
	CFileIO io;

	// ディスク空き容量分のファイルを作成する
	HANDLE hFile = NULL;
	bSuccess = io.FileOpen( &hFile, v.GetPath(), dwFlagsAndAttributes );
	if (!bSuccess)
	{
		return false;
	}

	// 空き容量を取得
	LONGLONG lFileSize = v.GetTotalNumberOfFreeBytes();

	// ファイルサイズを確定する
	bSuccess = SetFileSize( &hFile, lFileSize );
	log.AddResult(L"ファイルサイズを確定", bSuccess);
	log.Add(L"\tファイルサイズ", lFileSize);


	// セクタボリュームを取得
	const int bufSize = v.GetBytesPerSector() * iTimes;


	CWriteFilePart512 wf( bufSize, 0 );
	bSuccess = wf.Execute(&hFile, v);

	io.FileClose(&hFile);

	DWORD end = GetTickCount();
	log.Add(L"処理時間/秒", (end-start)/1000);

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T(""));
	TCHAR *path =  argc > 1 ? argv[1] : NULL ;

	::MessageBox( NULL, L"アタッチするならここ！", L"", MB_OK);


	CLog log;

#if 0
	// 最後512バイトごとに書き込む
	TryEndWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL);
	log.Add(L"");
	log.Add(L"");
	TryEndWrite512ByteNormal(path, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH);
#endif

#if 1
	// 先頭から512バイトごとに書き込む
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

	::MessageBox( NULL, L"結果確認", L"", MB_OK );
	
	return 0;
}


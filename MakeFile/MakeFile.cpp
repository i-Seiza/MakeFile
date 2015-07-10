// MakeFile.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "FileIO.h"
#include "Log.h"
#include "windows.h"
#include <WinIoCtl.h>
#include <rpc.h>
#include <map>


#include <tchar.h>
#include <iostream>
#include <locale.h>
#pragma comment( lib, "Rpcrt4.lib" )

void WriteData(LONGLONG pointer, LONGLONG totalSize);


void GetData()
{
	CFileIO io;
	HANDLE hFile = NULL;

	hFile = CreateFile(
		L"\\\\.\\PhysicalDrive0",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH,
		NULL
		);
	if (hFile==INVALID_HANDLE_VALUE)	return;


	DRIVE_LAYOUT_INFORMATION_EX *driveLayoutInfo = new DRIVE_LAYOUT_INFORMATION_EX[4096];
	
	DWORD BytesReturned;
	bool b = DeviceIoControl(
		hFile,
		IOCTL_DISK_GET_DRIVE_LAYOUT_EX, // dwIoControlCode
		NULL,                           // lpInBuffer
		0,                              // nInBufferSize
		driveLayoutInfo,           // output buffer
		sizeof(*driveLayoutInfo)*4096,         // size of output buffer
		&BytesReturned,      // number of bytes returned
		NULL );  // OVERLAPPED structure

	io.FileClose(&hFile);

	CLog log;
	log.Add( (boost::format("PartitionCount: %1%") % driveLayoutInfo->PartitionCount ).str().c_str());
	log.Add( (boost::format("PartitionStyle: %1%") %  driveLayoutInfo->PartitionStyle ).str().c_str());
	log.Add( (boost::format("Mbr.Signature: %1%") %  driveLayoutInfo->Mbr.Signature ).str().c_str());
	log.Add( L"Gpt.DiskId:", driveLayoutInfo->Gpt.DiskId);
	log.Add( (boost::format("Gpt.MaxPartitionCount: %1%") %  driveLayoutInfo->Gpt.MaxPartitionCount ).str().c_str());
	log.Add( (boost::format("Gpt.StartingUsableOffset: %1%") %  driveLayoutInfo->Gpt.StartingUsableOffset.QuadPart ).str().c_str());
	log.Add( (boost::format("Gpt.UsableLength: %1%") %  driveLayoutInfo->Gpt.UsableLength.QuadPart ).str().c_str());
	log.Add("--------------");
	for (int i = 0; i < driveLayoutInfo->PartitionCount; i++)
	{
		if(driveLayoutInfo->PartitionEntry[i].PartitionLength.QuadPart == 0 )	continue;
		if( driveLayoutInfo->PartitionEntry[i].Gpt.Attributes & GPT_BASIC_DATA_ATTRIBUTE_READ_ONLY)	continue;	// 読み取り専用ドライブ（予約済みエリアなど）の場合は情報取得を不要とする

		log.Add("");
		log.Add( (boost::format("PartitionNumber:%1%") %  driveLayoutInfo->PartitionEntry[i].PartitionNumber ).str().c_str());	//DWORD
		log.Add( (boost::format("PartitionStyle:%1%") %  driveLayoutInfo->PartitionEntry[i].PartitionStyle ).str().c_str());	//PARTITION_STYLE
		log.Add( (boost::format("StartingOffset:%1%") %  driveLayoutInfo->PartitionEntry[i].StartingOffset.QuadPart ).str().c_str());	//LARGE_INTEGER
		log.Add( (boost::format("PartitionLength:%1%") %  driveLayoutInfo->PartitionEntry[i].PartitionLength.QuadPart ).str().c_str());	//LARGE_INTEGER
		log.Add( (boost::format("RewritePartition:%1%") %  driveLayoutInfo->PartitionEntry[i].RewritePartition ).str().c_str());	//bool
		log.Add( (boost::format("Mbr.PartitionType:%1%") %  driveLayoutInfo->PartitionEntry[i].Mbr.PartitionType ).str().c_str());	//PARTITION_INFORMATION_MBR.byte
		log.Add( (boost::format("Mbr.BootIndicator:%1%") %  driveLayoutInfo->PartitionEntry[i].Mbr.BootIndicator ).str().c_str());	//PARTITION_INFORMATION_MBR.BootIndicator
		log.Add( (boost::format("Mbr.RecognizedPartition:%1%") %  driveLayoutInfo->PartitionEntry[i].Mbr.RecognizedPartition ).str().c_str());	//PARTITION_INFORMATION_MBR.bool
		log.Add( (boost::format("Mbr.HiddenSectors:%1%") %  driveLayoutInfo->PartitionEntry[i].Mbr.HiddenSectors ).str().c_str());	//PARTITION_INFORMATION_MBR.dword


		log.Add( L"Gpt.PartitionType:%1%", driveLayoutInfo->PartitionEntry[i].Gpt.PartitionType );	//PARTITION_INFORMATION_GPT.GUID
		log.Add( L"Gpt.PartitionId:%1%", driveLayoutInfo->PartitionEntry[i].Gpt.PartitionId );	//PARTITION_INFORMATION_GPT.GUID
		log.Add( (boost::format("Gpt.Attributes:%1%") %  driveLayoutInfo->PartitionEntry[i].Gpt.Attributes ).str().c_str());	//PARTITION_INFORMATION_GPT.DWORD64
		log.Add( (boost::format("Gpt.Name:%1%") %  driveLayoutInfo->PartitionEntry[i].Gpt.Name ).str().c_str());	//PARTITION_INFORMATION_GPT.wchar*

		
		std::map<RPC_WSTR, std::wstring> mData;
		mData.insert( std::make_pair<RPC_WSTR, std::wstring>((RPC_WSTR)L"ebd0a0a2-b9e5-4433-87c0-68b6b72699c7", L"PARTITION_BASIC_DATA_GUID") );
		mData.insert( std::make_pair<RPC_WSTR, std::wstring>((RPC_WSTR)L"00000000-0000-0000-0000-000000000000", L"PARTITION_ENTRY_UNUSED_GUID"));
		mData.insert( std::make_pair<RPC_WSTR, std::wstring>((RPC_WSTR)L"c12a7328-f81f-11d2-ba4b-00a0c93ec93b", L"PARTITION_SYSTEM_GUID"));
		mData.insert( std::make_pair<RPC_WSTR, std::wstring>((RPC_WSTR)L"e3c9e316-0b5c-4db8-817d-f92df00215ae", L"PARTITION_MSFT_RESERVED_GUID"));
		mData.insert( std::make_pair<RPC_WSTR, std::wstring>((RPC_WSTR)L"5808c8aa-7e8f-42e0-85d2-e1e90434cfb3", L"PARTITION_LDM_METADATA_GUID"));
		mData.insert( std::make_pair<RPC_WSTR, std::wstring>((RPC_WSTR)L"af9b60a0-1431-4f62-bc68-3311714a69ad", L"PARTITION_LDM_DATA_GUID"));
		mData.insert( std::make_pair<RPC_WSTR, std::wstring>((RPC_WSTR)L"de94bba4-06d1-4d40-a16a-bfd50179d6ac", L"PARTITION_MSFT_RECOVERY_GUID"));

		std::map<RPC_WSTR, std::wstring>::iterator it = mData.begin();
		while( it != mData.end() )
		{
			GUID guid;
			::UuidFromString((*it).first, &guid );
			if(guid == (driveLayoutInfo->PartitionEntry[i].Gpt.PartitionType) )		log.Add( (*it).second );


			it++;
		}


		if( driveLayoutInfo->PartitionEntry[i].Gpt.Attributes & GPT_ATTRIBUTE_PLATFORM_REQUIRED)	log.Add( L"GPT_ATTRIBUTE_PLATFORM_REQUIRED" );
		if( driveLayoutInfo->PartitionEntry[i].Gpt.Attributes & GPT_BASIC_DATA_ATTRIBUTE_NO_DRIVE_LETTER)	log.Add( L"GPT_BASIC_DATA_ATTRIBUTE_NO_DRIVE_LETTER" );
		if( driveLayoutInfo->PartitionEntry[i].Gpt.Attributes & GPT_BASIC_DATA_ATTRIBUTE_HIDDEN)	log.Add( L"GPT_BASIC_DATA_ATTRIBUTE_HIDDEN" );
		if( driveLayoutInfo->PartitionEntry[i].Gpt.Attributes & GPT_BASIC_DATA_ATTRIBUTE_SHADOW_COPY)	log.Add( L"GPT_BASIC_DATA_ATTRIBUTE_SHADOW_COPY" );
		if( driveLayoutInfo->PartitionEntry[i].Gpt.Attributes & GPT_BASIC_DATA_ATTRIBUTE_READ_ONLY)	log.Add( L"GPT_BASIC_DATA_ATTRIBUTE_READ_ONLY" );

		if( driveLayoutInfo->PartitionEntry[i].Gpt.Attributes )	continue;
		WriteData(driveLayoutInfo->PartitionEntry[i].StartingOffset.QuadPart, driveLayoutInfo->PartitionEntry[i].PartitionLength.QuadPart);

	}

	if (driveLayoutInfo)	delete []driveLayoutInfo;
	

}

void WriteData(LONGLONG pointer, LONGLONG totalSize)
{

	HANDLE hFile = NULL;

	hFile = CreateFile(
		L"\\\\.\\PhysicalDrive0",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH,
		NULL
		);
	if (hFile==INVALID_HANDLE_VALUE)	return;


	LARGE_INTEGER DistanceToMove;
	DistanceToMove.QuadPart = pointer;
	LARGE_INTEGER NewFilePointer;

	BOOL b = SetFilePointerEx(
		  hFile,
		  DistanceToMove,
		  &NewFilePointer,
		  FILE_BEGIN
	);
	if(!b)
	{
		CloseHandle(hFile);
		return;
	}

	LONGLONG bufSize = 512*1024*1024;
	LONGLONG size = min(bufSize, totalSize);
	BYTE *buf = (BYTE*)VirtualAlloc(NULL, bufSize, MEM_COMMIT, PAGE_READWRITE);
	while(totalSize > 0)
	{
		DWORD NumberOfBytesWritten;
		b = WriteFile(
		  hFile,                    // ファイルのハンドル
		  buf,                // データバッファ
		  size,     // 書き込み対象のバイト数
		  &NumberOfBytesWritten,  // 書き込んだバイト数
		  NULL        // オーバーラップ構造体のバッファ
		);
		totalSize -= NumberOfBytesWritten;
		if(!b)	break;
	}

	VirtualFree(buf, 0, MEM_FREE);
	CloseHandle(hFile);
}


int _tmain(int argc, _TCHAR* argv[])
{
	::MessageBox(NULL, L"", L"", MB_OK);
	GetData();


	return 0;
}


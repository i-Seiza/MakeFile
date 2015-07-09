// MakeFile.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "FileIO.h"



int _tmain(int argc, _TCHAR* argv[])
{
	::MessageBox(NULL, L"", L"", MB_OK);

	CFileIO io;
	HANDLE hFile = NULL;

	hFile = CreateFile(
		//L"\\\\.\\E:",
		L"\\\\.\\PhysicalDrive0",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH,
		NULL
		);
	if (hFile==INVALID_HANDLE_VALUE)	return 0;


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


	printf("PartitionCount: %d\n",driveLayoutInfo->PartitionCount);
	printf("PartitionStyle: %d\n", driveLayoutInfo->PartitionStyle);
	printf("Mbr: %d\n", driveLayoutInfo->Mbr);
	printf("Gpt: %d\n", driveLayoutInfo->Gpt);
	printf("--------------\n");
	for (int i = 0; i < driveLayoutInfo->PartitionCount; i++)
	{
		printf("パーティション番号:%d\n", i);
		printf("%PartitionStyle:%d\n", driveLayoutInfo->PartitionEntry[i].PartitionStyle);	//PARTITION_STYLE
		printf("StartingOffset:%I64X\n", driveLayoutInfo->PartitionEntry[i].StartingOffset.QuadPart);	//LARGE_INTEGER
		printf("PartitionLength:%I64X\n", driveLayoutInfo->PartitionEntry[i].PartitionLength.QuadPart);	//LARGE_INTEGER
		printf("PartitionNumber:%08LX\n", driveLayoutInfo->PartitionEntry[i].PartitionNumber);	//DWORD
		printf("RewritePartition:%d\n", driveLayoutInfo->PartitionEntry[i].RewritePartition);	//bool
		printf("Mbr.PartitionType:%d\n", driveLayoutInfo->PartitionEntry[i].Mbr.PartitionType);	//PARTITION_INFORMATION_MBR.byte
		printf("Mbr.BootIndicator:%d\n", driveLayoutInfo->PartitionEntry[i].Mbr.BootIndicator);	//PARTITION_INFORMATION_MBR.BootIndicator
		printf("Mbr.RecognizedPartition:%d\n", driveLayoutInfo->PartitionEntry[i].Mbr.RecognizedPartition);	//PARTITION_INFORMATION_MBR.bool
		printf("Mbr.HiddenSectors:%08LX\n", driveLayoutInfo->PartitionEntry[i].Mbr.HiddenSectors);	//PARTITION_INFORMATION_MBR.dword


		//printf("Gpt.PartitionType:%d\n", driveLayoutInfo->PartitionEntry[i].Gpt.PartitionType);	//PARTITION_INFORMATION_GPT.GUID
		//printf("Gpt.PartitionId:%d\n", driveLayoutInfo->PartitionEntry[i].Gpt.PartitionId);	//PARTITION_INFORMATION_GPT.GUID
		printf("Gpt.Attributes:%d\n", driveLayoutInfo->PartitionEntry[i].Gpt.Attributes);	//PARTITION_INFORMATION_GPT.DWORD64
		printf("Gpt.Name:%s\n", driveLayoutInfo->PartitionEntry[i].Gpt.Name);	//PARTITION_INFORMATION_GPT.wchar*

	}

	if (driveLayoutInfo)	delete []driveLayoutInfo;
	

	io.FileClose(&hFile);

	return 0;
}


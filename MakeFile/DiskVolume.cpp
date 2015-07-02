#include "stdafx.h"
#include "DiskVolume.h"
#include "Log.h"



CDiskVolume::CDiskVolume()
{
	SetPath(L"E:\\dummy.txt");
}

void CDiskVolume::SetPath(TCHAR *_path)
{
	swprintf_s(path, MAX_PATH, _path );
	_wsplitpath_s( path, drive, MAX_PATH, NULL, 0, NULL, 0, NULL, 0 );
	wcscat(drive, L"\\");
}

// �w�肳�ꂽ�h���C�u�̏����擾����
bool CDiskVolume::GetDiskVolumeInfo(TCHAR *_path)
{
	if (_path != NULL)
	{
		SetPath(_path);
	}

	// �_�~�[�t�@�C�����폜����
	DeleteFile(path);

	// ���O
	CLog log;

	// �擾����
	BOOL bSuccess = GetDiskFreeSpaceEx(
		drive,
		(PULARGE_INTEGER)&FreeBytesAvailable,    // �Ăяo���������p�ł���o�C�g��
		(PULARGE_INTEGER)&TotalNumberOfBytes,    // �f�B�X�N�S�̂̃o�C�g��
		(PULARGE_INTEGER)&TotalNumberOfFreeBytes // �f�B�X�N�S�̂̋󂫃o�C�g��
		);
	log.AddResult(L"�f�B�X�N�e�ʎ擾", bSuccess != 0);

	if (bSuccess == 0)
	{
		return false;
	}

	log.Add(L"\t�f�B�X�N�`�F�b�N����h���C�u", drive);
	log.Add(L"\t�Ăяo���������p�ł���o�C�g��", FreeBytesAvailable);
	log.Add(L"\t�f�B�X�N�S�̂̃o�C�g��", TotalNumberOfBytes);
	log.Add(L"\t�f�B�X�N�S�̂̋󂫃o�C�g��",TotalNumberOfFreeBytes);
	log.Add(L"----------");

	bSuccess = GetDiskFreeSpace(
		drive,					  // ���[�g�p�X
		&SectorsPerCluster,     // �N���X�^������̃Z�N�^��
		&BytesPerSector,        // �Z�N�^������̃o�C�g��
		&NumberOfFreeClusters,  // �󂫃N���X�^��
		&TotalNumberOfClusters  // �S�N���X�^��
		);

	log.AddResult(L"GetDiskFreeSpace", bSuccess != 0);
	log.Add(L"\t�N���X�^������̃Z�N�^��", SectorsPerCluster);
	log.Add(L"\t�Z�N�^������̃o�C�g��", BytesPerSector);
	log.Add(L"\t�󂫃N���X�^��",NumberOfFreeClusters);
	log.Add(L"\t�S�N���X�^��",TotalNumberOfClusters);
	log.Add(L"----------");




	if (bSuccess == 0)
	{
		return false;
	}


	TCHAR VolumeNameBuffer[_MAX_PATH];        // �{�����[�����o�b�t�@
	DWORD VolumeSerialNumber;     // �{�����[���̃V���A���ԍ�
	DWORD MaximumComponentLength; // �t�@�C�����̍ő�̒���
	DWORD FileSystemFlags;        // �t�@�C���V�X�e���̃I�v�V����
	TCHAR FileSystemNameBuffer[_MAX_PATH];    // �t�@�C���V�X�e�������i�[����o�b�t�@


	// �w�肳�ꂽ���[�g�f�B���N�g���̃t�@�C���V�X�e���ƃ{�����[���Ɋւ�������擾���܂��B
	bSuccess = GetVolumeInformation(
		drive,           // ���[�g�f�B���N�g��
		VolumeNameBuffer,        // �{�����[�����o�b�t�@
		sizeof(VolumeNameBuffer),            // ���O�o�b�t�@�̃T�C�Y
		&VolumeSerialNumber,     // �{�����[���̃V���A���ԍ�
		&MaximumComponentLength, // �t�@�C�����̍ő�̒���
		&FileSystemFlags,        // �t�@�C���V�X�e���̃I�v�V����
		FileSystemNameBuffer,    // �t�@�C���V�X�e�������i�[����o�b�t�@
		sizeof(FileSystemNameBuffer)	// �t�@�C���V�X�e�������i�[����o�b�t�@�̃T�C�Y
		);
	log.AddResult(L"GetVolumeInformation", bSuccess != 0);
	log.Add(L"\t�{�����[����",VolumeNameBuffer);
	log.Add(L"\t�{�����[���̃V���A���ԍ�", VolumeSerialNumber);
	log.Add(L"\t�t�@�C�����̍ő�̒���",MaximumComponentLength);
	log.Add(L"\t�t�@�C���V�X�e���̃I�v�V����",FileSystemFlags);
	log.Add(L"\t�t�@�C���V�X�e����", FileSystemNameBuffer);
	log.Add(L"----------");

	if (bSuccess == 0)
	{
		return false;
	}

	bSupportsSparseFiles = FileSystemFlags & FILE_SUPPORTS_SPARSE_FILES;


	return true;
}


TCHAR * CDiskVolume::GetPath()
{
	return path;
}



TCHAR * CDiskVolume::GetDrive()
{
	return drive;
}

DWORD CDiskVolume::GetBytesPerSector()
{
	return BytesPerSector;
}

DWORD CDiskVolume::GetSectorsPerCluster()
{
	return SectorsPerCluster;
}


DWORD CDiskVolume::GetNumberOfFreeClusters()
{
	return NumberOfFreeClusters;
}

unsigned __int64 CDiskVolume::GetTotalNumberOfBytes()
{
	return TotalNumberOfBytes;
}

unsigned __int64 CDiskVolume::GetTotalNumberOfFreeBytes()
{
	return TotalNumberOfFreeBytes;
}

bool CDiskVolume::IsSupportsSparseFiles()
{
	return bSupportsSparseFiles;
}


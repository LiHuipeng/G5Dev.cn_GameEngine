#pragma once

class CCrc32
{
public:
	CCrc32(void);
	~CCrc32(void);

public:
	unsigned long CalculateCrc32(const void* pbuffer, unsigned long size);

protected:
	unsigned long Reflect(unsigned long ref, char ch);
	void InitCrc32Table();

private:
	unsigned long m_Crc32Table[256];		//正规查询表
};

/*
	使用说明:

	HANDLE IniFile;
	IniFile=CreateFile("MyCrc111.exe", GENERIC_READ, FILE_SHARE_READ, NULL,
	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(IniFile==INVALID_HANDLE_VALUE) return FALSE;
	DWORD nFileDataSize=GetFileSize(IniFile, NULL);
	BYTE* pFileData=new BYTE[nFileDataSize];
	DWORD nRead=0;
	ReadFile(IniFile,pFileData,nFileDataSize,&nRead,NULL);
	CloseHandle(IniFile);

	if (nRead>0)
	{
		CCrc32 crc32;
		ULONG crc = crc32.CalculateCrc32(pFileData,nRead);
		printf("crc=0x%X  crc=%u\n",crc,crc);
	}

	if (pFileData)
	{
		delete[] pFileData;
		pFileData = NULL;
	}

*/
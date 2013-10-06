// 付备 创建于 2013年
// huipeng_lhp@163.com 修改于 2013-09-18
#pragma once


#pragma pack(1)

#define MAX_CANVAS_WIDTH  2048
#define MAX_CANVAS_HEIGHT 1024

struct SDpkFileHeader
{
	char	FileMark[6];	// DPK4
	ULONG	PasswordCrc32;
};

#define MAX_DPK_ITEM_PATH	100
struct SDpkFileItem
{
	WCHAR	szItemPath[MAX_DPK_ITEM_PATH];
	ULONG	ulDataCrc32; // 加密前Data的计算值
	DWORD	dwDataSize;
	//Data...
};

struct STileImageInfo
{
	int canvas;
	int x;
	int y;
	int w;
	int h;
};

#pragma pack()


struct SDpkMemItem
{
	WCHAR	szItemPath[MAX_DPK_ITEM_PATH];
	ULONG	ulDataCrc32; // 加密前Data的计算值
	DWORD	dwDataSize;
	int		nFileCursor;
	//Data...
};


inline void XorEncode(void* pData, UINT nLen, LPCWSTR szKey)
{
	BYTE* p=(BYTE*)pData;
	UINT nKeyLen = wcslen(szKey);
	UINT nIndex = 0;

	for (UINT i = 0; i < nLen; i++)
	{
		p[i] ^= szKey[nIndex];
		nIndex++;
		nIndex = nIndex%nKeyLen;
	}
}


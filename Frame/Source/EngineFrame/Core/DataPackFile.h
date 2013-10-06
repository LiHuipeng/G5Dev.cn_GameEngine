#pragma once
#include "EngineResource.h"
#include "ISysFunction.h"
#include "DpkFileDefine.h"


class CDataPackFile : public CEngineResource
{
public:
	CDataPackFile(void);
	virtual ~CDataPackFile(void);

	BOOL Load(ISysFile* pFile, LPCWSTR szDpkPath, LPCWSTR szDpkPassword);

	virtual BOOL ReadItemData(LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize);
	virtual BOOL WriteItemData(LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize);
	virtual BOOL IsItemExists(LPCWSTR szItemPath, BOOL& bItemExists) const;
	virtual BOOL GetItemSize(LPCWSTR szItemPath, DWORD& dwItemSize) const;

private:
	ISysFile*	m_pFile;
	WCHAR		m_szDpkFilePath[MAX_PATH];
	LPWSTR		m_pKey;
	std::list<SDpkMemItem>	m_DpkMemItemList;
};


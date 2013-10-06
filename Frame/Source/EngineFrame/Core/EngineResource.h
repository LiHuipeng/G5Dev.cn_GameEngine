#pragma once
#include "IEngineFrame.h"


class CEngineResource
{
public:
	CEngineResource(void);
	virtual ~CEngineResource(void);

	virtual BOOL ReadItemData(LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize);
	virtual BOOL WriteItemData(LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize);
	virtual BOOL IsItemExists(LPCWSTR szItemPath, BOOL& bItemExists) const;
	virtual BOOL GetItemSize(int nItemIndex, SIZE& ItemSize) const;
	virtual BOOL GetItemSize(LPCWSTR szItemPath, DWORD& dwItemSize) const;
	virtual int  GetItemCount(void) const;

	virtual BOOL Draw(LPCWSTR szText, const SEngineDrawParam* pDrawParam) const;
	virtual BOOL Draw(int nItemIndex, const SEngineDrawParam* pDrawParam) const;
	virtual void Play(BOOL bLoop);
	virtual void Stop();
	virtual void Update();

	WCHAR	m_szPackPath[MAX_PATH];
	int		m_nReferenceCount;
};


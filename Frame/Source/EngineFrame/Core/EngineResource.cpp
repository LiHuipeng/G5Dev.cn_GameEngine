#include "stdafx.h"
#include "EngineResource.h"


CEngineResource::CEngineResource(void)
{
	ZeroMemory(m_szPackPath, sizeof(m_szPackPath));
	m_nReferenceCount=0;
}


CEngineResource::~CEngineResource(void)
{
}

BOOL CEngineResource::ReadItemData( LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize )
{
	ASSERT(FALSE && _T("功能未实现"));
	return FALSE;
}

BOOL CEngineResource::WriteItemData( LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize )
{
	ASSERT(FALSE && _T("功能未实现"));
	return FALSE;
}

BOOL CEngineResource::IsItemExists( LPCWSTR szItemPath, BOOL& bItemExists ) const
{
	ASSERT(FALSE && _T("功能未实现"));
	return FALSE;
}

BOOL CEngineResource::GetItemSize( LPCWSTR szItemPath, DWORD& dwItemSize ) const
{
	ASSERT(FALSE && _T("功能未实现"));
	return FALSE;
}

BOOL CEngineResource::GetItemSize( int nItemIndex, SIZE& ItemSize ) const
{
	ASSERT(FALSE && _T("功能未实现"));
	return FALSE;
}

BOOL CEngineResource::Draw( int nItemIndex, const SEngineDrawParam* pDrawParam ) const
{
	ASSERT(FALSE && _T("功能未实现"));
	return FALSE;
}

BOOL CEngineResource::Draw( LPCWSTR szText, const SEngineDrawParam* pDrawParam ) const
{
	ASSERT(FALSE && _T("功能未实现"));
	return FALSE;
}

int CEngineResource::GetItemCount( void ) const
{
	ASSERT(FALSE && _T("功能未实现"));
	return 0;
}

void CEngineResource::Play( BOOL bLoop )
{
	ASSERT(FALSE && _T("功能未实现"));
}

void CEngineResource::Stop()
{
	ASSERT(FALSE && _T("功能未实现"));
}

void CEngineResource::Update()
{
}


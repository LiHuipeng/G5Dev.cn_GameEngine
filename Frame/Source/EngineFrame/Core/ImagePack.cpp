#include "stdafx.h"
#include "ImagePack.h"
#include "MacroUtility.h"

#include "MemoryCheck.h"



CImagePack::CImagePack()
{
	m_pTileImageInfo=NULL;
}


CImagePack::~CImagePack(void)
{
	for(DWORD i=0; i<m_ImageVector.size(); i++)
	{
		IEngineImage* pImage=m_ImageVector[i];
		SAFE_RELEASE(pImage);
		m_ImageVector[i]=NULL;
	}

	SAFE_DELETE_ARRAY(m_pTileImageInfo);
}

BOOL CImagePack::Load( LPCWSTR szPackPath, CEngineResource* pDpkResource, IEngineRender* pRender )
{
	BOOL bPackExists=FALSE;
	pDpkResource->IsItemExists(szPackPath, bPackExists);
	if(!bPackExists) return FALSE;

	BYTE* pData=NULL;
	DWORD dwSize=0;	
	WCHAR path[MAX_PATH];

	BOOL bIsMergeMode=FALSE;
	_stprintf_s(path, _T("%s\\Merge.bin"), szPackPath);
	pDpkResource->IsItemExists(path, bIsMergeMode);

	if(bIsMergeMode) // 这是merge模式
	{
		DWORD dwItemSize=0;
		if(!pDpkResource->GetItemSize(path, dwItemSize)) return FALSE;

		m_nTileImageCount=dwItemSize/sizeof(STileImageInfo);
		m_pTileImageInfo=new STileImageInfo[m_nTileImageCount];

		// 填写 m_pTileImageInfo
		_stprintf_s(path, _T("%s\\Merge.bin"), szPackPath);
		BOOL b=pDpkResource->ReadItemData(path, m_pTileImageInfo, sizeof(STileImageInfo)*m_nTileImageCount);
		ASSERT(b);

		for(int i=0; i<999; i++)
		{
			_stprintf_s(path, _T("%s\\m%02d.png"), szPackPath, i);
			DWORD dwItemSize=0;
			if(!pDpkResource->GetItemSize(path, dwItemSize)) break;
			BYTE* pItemData=new BYTE[dwItemSize];
			if(!pDpkResource->ReadItemData(path, pItemData, dwItemSize)) { delete[] pItemData; break; }

			// 加载图片
			ASSERT(pRender);
			IEngineImage* pImage=pRender->CreateImage(pItemData, dwItemSize, 0);
			if(pImage) m_ImageVector.push_back(pImage);

			delete[] pItemData;
		}
	}
	else
	{
		// 获得图片数量
		m_nTileImageCount=0;
		for(int i=0; i<999; i++)
		{
			_stprintf_s(path, _T("%s\\%03d.png"), szPackPath, i);
			BOOL bItemExists;
			if(!pDpkResource->IsItemExists(path, bItemExists)) break;
			if(!bItemExists) break;
			m_nTileImageCount++;
		}

		m_pTileImageInfo=new STileImageInfo[m_nTileImageCount];

		for(int i=0; i<m_nTileImageCount; i++)
		{
			_stprintf_s(path, _T("%s\\%03d.png"), szPackPath, i);
			DWORD dwItemSize=0;
			if(!pDpkResource->GetItemSize(path, dwItemSize)) return FALSE;
			BYTE* pItemData=new BYTE[dwItemSize];
			if(!pDpkResource->ReadItemData(path, pItemData, dwItemSize)) { delete[] pItemData; break; }

			// 加载图片
			ASSERT(pRender);
			IEngineImage* pImage=pRender->CreateImage(pItemData, dwItemSize, 0);
			if(pImage) m_ImageVector.push_back(pImage);

			// 填写 m_pTileImageInfo
			m_pTileImageInfo[i].canvas=i;
			m_pTileImageInfo[i].x=0;
			m_pTileImageInfo[i].y=0;
			m_pTileImageInfo[i].w=pImage->GetWidth();
			m_pTileImageInfo[i].h=pImage->GetHeight();

			delete[] pItemData;
		}
	}

	_tcscpy_s(m_szPackPath, MAX_PATH, szPackPath);
	return TRUE;
}

BOOL CImagePack::Draw( int nItemIndex, const SEngineDrawParam* pDrawParam ) const
{
	LONG x=pDrawParam->x;
	LONG y=pDrawParam->y;
	LONG w=pDrawParam->w;
	LONG h=pDrawParam->h;
	LONG sx=pDrawParam->sx;
	LONG sy=pDrawParam->sy;
	LONG sw=pDrawParam->sw;
	LONG sh=pDrawParam->sh;
	DWORD color=pDrawParam->color;
	if(w<0 || h<0 || sw<0 || sh<0 || sx<0 || sy<0) return FALSE;

	if(!pDrawParam->flag) color=0xffffffff;

	SIZE TileImageSize={m_pTileImageInfo[nItemIndex].w, m_pTileImageInfo[nItemIndex].h};

	// 从原图上取局部进行描画时一定要传入 sx、sy、sw、sh 这4个值的
	if(!w) w=TileImageSize.cx;
	if(!h) h=TileImageSize.cy;
	if(!sw) sw=TileImageSize.cx;
	if(!sh) sh=TileImageSize.cy;

	if(sw>TileImageSize.cx || sh>TileImageSize.cy) return FALSE;

	sx+=m_pTileImageInfo[nItemIndex].x;
	sy+=m_pTileImageInfo[nItemIndex].y;

	int nCanvas=m_pTileImageInfo[nItemIndex].canvas;
	ASSERT(DWORD(nCanvas)<m_ImageVector.size());
	m_ImageVector[nCanvas]->Draw(x, y, w, h, sx, sy, sw, sh, color);
	return TRUE;
}

int CImagePack::GetItemCount( void ) const
{
	return m_nTileImageCount;
}

BOOL CImagePack::GetItemSize( int nItemIndex, SIZE& ItemSize ) const
{
	ASSERT(nItemIndex>=0 && nItemIndex<m_nTileImageCount);
	ItemSize.cx=m_pTileImageInfo[nItemIndex].w;
	ItemSize.cy=m_pTileImageInfo[nItemIndex].h;
	return TRUE;
}

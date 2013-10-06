#include "stdafx.h"
#include "EngineFrameImp.h"
#include "DataPackFile.h"
#include "DataPackFolder.h"
#include "ImagePack.h"
#include "FontPack.h"
#include "Animation.h"

#include "MacroUtility.h"
#include "MemoryCheck.h"


IEngineFrame* EngineFrameCreate(DWORD dwVersion)
{
	if(ENGINEFRAME_VERSION!=dwVersion) return NULL;
	return new CEngineFrameImp;
}

CEngineFrameImp::CEngineFrameImp()
{
	m_pRender=EngineRenderCreate();
	m_pSound=EngineSoundCreate();
	m_pFile=SysFileCreate(SYSFUNCTION_VERSION);
	ZeroMemory(m_szLastszErrorDescription, sizeof(m_szLastszErrorDescription));
}

CEngineFrameImp::~CEngineFrameImp()
{
	SAFE_RELEASE(m_pRender);
	SAFE_RELEASE(m_pSound);
	SAFE_RELEASE(m_pFile);
}

void CEngineFrameImp::Release()
{
	delete this;
}

BOOL CEngineFrameImp::EngineInit( const SEngineInitParam* pEngineInitParam )
{
//	_CrtSetBreakAlloc(180);

	m_EngineInitParam=*pEngineInitParam;
	m_pRender->Init(pEngineInitParam->hCanvasWnd, pEngineInitParam->nCanvasCX, pEngineInitParam->nCanvasCY, pEngineInitParam->nViewCX, pEngineInitParam->nViewCY);
	m_pSound->Init(pEngineInitParam->hAppWnd);

	return TRUE;
}

BOOL CEngineFrameImp::EngineRenderBegin()
{
	if(!m_pRender) return FALSE;

	m_pRender->RenderBegin();
	return TRUE;
}

BOOL CEngineFrameImp::EngineRenderEnd()
{
	if(!m_pRender) return FALSE;

	m_pRender->RenderEnd();
	return TRUE;
}

const SEngineInitParam* CEngineFrameImp::GetEngineInitParam() const
{
	return &m_EngineInitParam;
}

void CEngineFrameImp::EngineGetLastError( LPWSTR szErrorDescription, UINT nErrorSize )
{
	wcscpy_s(szErrorDescription, nErrorSize, m_szLastszErrorDescription);
}

void CEngineFrameImp::EngineSetLastError( LPCWSTR szErrorDescription )
{
	wcscpy_s(m_szLastszErrorDescription, szErrorDescription);
}

// CImagePack* CEngineFrameImp::ImagePackCreate(CDataPack* pDataPack, LPCWSTR szDpkSubPath)
// {
// 	CImagePack* p=m_ImagePackPool.FindPtr(szDpkSubPath);
// 	if(p) // 找到了之前创建过的相同资源
// 	{
// 		m_ImagePackPool.AddPtrRef(szDpkSubPath);
// 		return p;
// 	}
// 
// 	LPBYTE pDataPtr=NULL;
// 	DWORD  dwDataSize=0;
// 	WCHAR szPath[MAX_PATH];
// 	swprintf_e(szPath, _T("%s\\MergeInfo.bin"), szDpkSubPath);
// 
// 	CImagePack* pNew = NULL;
// 	BOOL bResult = FALSE;
// 	if (pDataPack->ExtractFileDataFromPack(szPath, pDataPtr, dwDataSize))
// 	{
// 		pNew = new CImagePackMerge;
// 		bResult = ((CImagePackMerge*)pNew)->Create(pDataPack, szDpkSubPath, m_pRender);
// 	}
// 	else
// 	{
// 		pNew = new CImagePackPng;
// 		bResult = ((CImagePackPng*)pNew)->Create(pDataPack, szDpkSubPath, m_pRender);
// 	}
// 	if (pNew)
// 	{
// 		wcscpy_s(pNew->m_szDpkSubPath, szDpkSubPath);
// 	}
// 
// 	if(bResult) 
// 		m_ImagePackPool.AddPtr(szDpkSubPath, pNew);
// 
// 	return pNew;
//}


// CImagePack* CEngineFrameImp::ImagePackCreate( LPCVOID pImageData, DWORD dwImageSize )
// {
// 	static DWORD dwIndex=0;
// 	WCHAR szDpkSubPath[MAX_PATH];
// 	_stprintf_s(szDpkSubPath, _T("ImagePackMem[%u]"), dwIndex);
// 	dwIndex++;
// 
// 	CImagePackMem* pNew=new CImagePackMem;
// 	wcscpy_s(pNew->m_szDpkSubPath, szDpkSubPath);
// 	BOOL b=pNew->Create(m_pRender, pImageData, dwImageSize);
// 	if(b) m_ImagePackPool.AddPtr(szDpkSubPath, pNew);
// 	return pNew;
//}


// void CEngineFrameImp::ImagePackRelease( CImagePack* pImagePack )
// {
// 	if(!pImagePack) return;
// 
// 	WCHAR szDpkSubPath[MAX_PATH];
// 	wcscpy_s(szDpkSubPath, pImagePack->m_szDpkSubPath);
// 	CImagePack* p=m_ImagePackPool.RemovePtr(pImagePack);
// 	if(p)
// 	{
// 		delete p;
// 	}
// }


// BOOL CEngineFrameImp::ImagePackDraw(CImagePack* pImagePack, UINT nImage, const SEngineDrawParam* pDrawParam) const
// {
// 	if(!pImagePack) return FALSE;
// 
// 	LONG x=pDrawParam->x;
// 	LONG y=pDrawParam->y;
// 	LONG w=pDrawParam->w;
// 	LONG h=pDrawParam->h;
// 	LONG sx=pDrawParam->sx;
// 	LONG sy=pDrawParam->sy;
// 	LONG sw=pDrawParam->sw;
// 	LONG sh=pDrawParam->sh;
// 	DWORD color=pDrawParam->color;
// 	if(w<0 || h<0 || sw<0 || sh<0 || sx<0 || sy<0) return FALSE;
// 
// 	if(!pDrawParam->flag) color=0xffffffff;
// 
// 	SIZE ImageSize=pImagePack->GetImageSize(nImage);
// 	if(!w) w=ImageSize.cx;
// 	if(!h) h=ImageSize.cy;
// 	if(!sw) sw=ImageSize.cx;	// 从原图上取局部进行描画时一定要传入 sx、sy、sw、sh 这4个值的
// 	if(!sh) sh=ImageSize.cy;
// 
// 	return pImagePack->DrawImage(nImage, x, y, w, h, sx, sy, sw, sh, color);
// }

// SIZE CEngineFrameImp::ImagePackGetImageSize( CImagePack* pImagePack, UINT nImage ) const
// {
// 	ASSERT(pImagePack);
// 	return pImagePack->GetImageSize(nImage);
// }

// int CEngineFrameImp::ImagePackGetImageCount( CImagePack* pImagePack ) const
// {
// 	ASSERT(pImagePack);
// 	return pImagePack->GetImageCount();
// }

// CFontPack* CEngineFrameImp::FontPackCreate(CDataPack* pDataPack, LPCWSTR szDpkSubPath)
// {
// 	CFontPack* p=NULL;
// 	if(m_FontPackPool.CreateObject(&p, szDpkSubPath))
// 	{
// 		BOOL b=p->Create(pDataPack, szDpkSubPath, this);
// 		ASSERT(b);
// 	}
// 	return p;
// }


// void CEngineFrameImp::FontPackRelease( CFontPack* pFontPack )
// {
// 	m_FontPackPool.ReleaseObject(pFontPack);
// }


// BOOL CEngineFrameImp::FontPackDraw(CFontPack* pFontPack, DWORD dwColor, UINT nFormat, LPCWSTR szText, LONG x, LONG y, LONG cx, LONG cy) const
// {
// 	ASSERT(szText);
// 	if(!pFontPack) return FALSE;
// 	RECT rec;
// 	rec.left=x;
// 	rec.top=y;
// 	rec.right=rec.left+cx;
// 	rec.bottom=rec.top+cy;
// 	pFontPack->DrawText(szText,rec, dwColor, nFormat);
// }

// CAnimation* CEngineFrameImp::AnimationCreate(CDataPack* pDataPack, LPCWSTR szSubPath)
// {
// 	ASSERT(20!=sizeof(CAnimation));
// 	CAnimation* p=new CAnimation;
// 	if(p->Create(this,pDataPack, szSubPath))
// 	{
// 		m_AnimationList.push_back(p);
// 		return p;
// 	}
// }


// void CEngineFrameImp::AnimationRelease( CAnimation* pAnimation )
// {
// 	if(!pAnimation) return;
// 
// 	CAnimation* p;
// 	std::list<CAnimation*>::iterator iter;
// 	for(iter=m_AnimationList.begin(); iter!=m_AnimationList.end(); iter++)
// 	{
// 		p=*iter;
// 		if(p == pAnimation)
// 		{
// 			p->Release();
// 			SAFE_DELETE(p);
// 			m_AnimationList.erase(iter);
// 			return;
// 		}
// 	}
// }


// BOOL CEngineFrameImp::AnimationDraw(CAnimation* pAnimation, const SEngineDrawParam* pDrawParam)
// {
// 	if(!pAnimation) return FALSE;
// 	if(!pAnimation->m_pImagePack) return FALSE;
// 	return ImagePackDraw(pAnimation->m_pImagePack, pAnimation->m_nPlayCurrentAniFrame, pDrawParam);
// }

// void CEngineFrameImp::AnimationPlay( CAnimation* pAnimation, BOOL bLoop )
// {
// 	ASSERT(pAnimation);
// 	pAnimation->Play(bLoop);
// }

// void CEngineFrameImp::AnimationStop( CAnimation* pAnimation )
// {
// 	ASSERT(pAnimation);
// 	pAnimation->Stop();
// }

// CSound* CEngineFrameImp::SoundCreate( LPCWSTR szFileName, BOOL bStreaming )
// {
// 	ASSERT(m_pSound);
// 	CSound* p=m_pSound->SoundCreate(szFileName, bStreaming);
// 	if(p)
// 	{
// 		m_SoundList.push_back(p);
// 		return p;
// 	}
// }

// void CEngineFrameImp::SoundPlay( CSound* pSound, BOOL bLoop )
// {
// 	ASSERT(m_pSound);
// 	m_pSound->SoundPlay(pSound, bLoop);
// }

// BOOL CEngineFrameImp::AnimationIsPlaying( CAnimation* pAnimation ) const
// {
// 	ASSERT(pAnimation);
// 	return pAnimation->IsPlaying();
// }

// SIZE CEngineFrameImp::AnimationGetImageSize( CAnimation* pAnimation ) const
// {
// 	ASSERT(pAnimation);
// 	ASSERT(pAnimation->m_pImagePack);
// 	return pAnimation->m_pImagePack->GetImageSize(0);
// }

// void CEngineFrameImp::EngineViewSizeChanged( int nViewCX, int nViewCY )
// {
// 	m_EngineInitParam.nViewCX = nViewCX;
// 	m_EngineInitParam.nViewCY = nViewCY;
// }


BOOL CEngineFrameImp::ResourceRelease( int nResIndex )
{
	DWORD dwResIndex=nResIndex;
	if(dwResIndex>=m_ResourceVector.size()) return FALSE;

	CEngineResource* p=m_ResourceVector[nResIndex];
	if(!p) return FALSE;
	
	if (0 == m_ResourceVector[nResIndex]->m_nReferenceCount)
	{
		delete p;
		m_ResourceVector[nResIndex]=NULL;
	}
	else
	{
		m_ResourceVector[nResIndex]->m_nReferenceCount--;
	}
	return TRUE;
}

BOOL CEngineFrameImp::DataPackCreate( int& nResIndex, LPCWSTR szPackPath, LPCWSTR szPassword )
{
	CEngineResource* pResource=NULL;

	LPCWSTR pSubStr=_tcsstr(szPackPath, _T(".dpk"));
	if(pSubStr) // 找到了匹配项
	{
		CDataPackFile* p;
		p=new CDataPackFile;
		if(!p->Load(m_pFile, szPackPath, szPassword))
		{
			delete p;
			return FALSE;
		}
		pResource=p;
	}
	else
	{
		CDataPackFolder* p;
		p=new CDataPackFolder;
		if(!p->Open(m_pFile, szPackPath))
		{
			delete p;
			return FALSE;
		}
		pResource=p;
	}

	AddResourceToVector(nResIndex, pResource);
	return TRUE;
}

BOOL CEngineFrameImp::ImagePackCreate( int& nResIndex, LPCWSTR szItemPath, int nDpkIndex )
{
	CEngineResource* pDpkResource=NULL;
	if(!GetValidResourcePtr(nDpkIndex, pDpkResource)) return FALSE;

	for(UINT i=0; i<m_ResourceVector.size(); i++)
	{
		ASSERT(m_ResourceVector[i]->m_szPackPath[1]!=_T(':'));
		if(_tcscmp(m_ResourceVector[i]->m_szPackPath, szItemPath)==0)
		{
			m_ResourceVector[i]->m_nReferenceCount++;
			nResIndex=i;
			return TRUE;
		}
	}

	CImagePack* pImagePack=new CImagePack;
	if(!pImagePack->Load(szItemPath, pDpkResource, m_pRender))
	{
		delete pImagePack;
		return FALSE;
	}

	AddResourceToVector(nResIndex, pImagePack);

	return TRUE;
}

BOOL CEngineFrameImp::GetValidResourcePtr( int nResIndex, CEngineResource*& pResource ) const
{
	DWORD dwResIndex=nResIndex;
	if(dwResIndex>=m_ResourceVector.size()) return FALSE;

	pResource=m_ResourceVector[nResIndex];
	if(!pResource) return FALSE;

	return TRUE;
}

void CEngineFrameImp::AddResourceToVector( int& nResIndex, CEngineResource* pResource )
{
	int n1=m_ResourceVector.size();
	m_ResourceVector.push_back(pResource);
	int n2=m_ResourceVector.size();
	ASSERT(n1+1==n2);
	nResIndex=n1;
}


BOOL CEngineFrameImp::ResourceDraw( int nResIndex, LPCWSTR szText, const SEngineDrawParam* pDrawParam ) const
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	return pResource->Draw(szText, pDrawParam);
}

BOOL CEngineFrameImp::ResourceDraw( int nResIndex, int nItemIndex, const SEngineDrawParam* pDrawParam ) const
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	return pResource->Draw(nItemIndex, pDrawParam);
}


BOOL CEngineFrameImp::AnimationCreate( int& nResIndex, LPCWSTR szItemPath, int nDpkIndex )
{
	CEngineResource* pDpkResource=NULL;
	if(!GetValidResourcePtr(nDpkIndex, pDpkResource)) return FALSE;

	CAnimation* pAnimation=new CAnimation;
	if(!pAnimation->Load(szItemPath, nDpkIndex, this))
	{
		delete pAnimation;
		return FALSE;
	}

	AddResourceToVector(nResIndex, pAnimation);
	return TRUE;
}

int CEngineFrameImp::ResourceGetItemCount( int nResIndex ) const
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	return pResource->GetItemCount();
}

BOOL CEngineFrameImp::ResourceGetItemSize( int nResIndex, LPCWSTR szItemPath, DWORD& dwItemSize ) const
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	return pResource->GetItemSize(szItemPath, dwItemSize);
}

BOOL CEngineFrameImp::ResourceGetItemSize( int nResIndex, int nItemIndex, SIZE& ItemSize ) const
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	return pResource->GetItemSize(nItemIndex, ItemSize);
}

BOOL CEngineFrameImp::ResourcePlay( int nResIndex, BOOL bLoop )
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	pResource->Play(bLoop);
	return TRUE;
}

BOOL CEngineFrameImp::ResourceStop( int nResIndex )
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	pResource->Stop();
	return TRUE;
}

void CEngineFrameImp::EngineUpdate()
{
	std::list<CAnimation*>::const_iterator iter = m_PlayingAnimationList.begin();
	while (iter != m_PlayingAnimationList.end())
	{
		(*iter)->Update();
		iter++;
	}
}

BOOL CEngineFrameImp::FontPackCreate( int& nResIndex, LPCWSTR szItemPath, int nDpkIndex )
{
	CEngineResource* pDpkResource=NULL;
	if(!GetValidResourcePtr(nDpkIndex, pDpkResource)) return FALSE;

	CFontPack* pFontPack=new CFontPack;
	if(!pFontPack->Load(szItemPath, nDpkIndex, this))
	{
		delete pFontPack;
		return FALSE;
	}

	AddResourceToVector(nResIndex, pFontPack);
	return TRUE;
}

BOOL CEngineFrameImp::ResourceReadItemData( int nResIndex, LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize )
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	return pResource->ReadItemData(szItemPath, pItemData, dwItemSize);
}

BOOL CEngineFrameImp::ResourceWriteItemData( int nResIndex, LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize )
{
	CEngineResource* pResource=NULL;
	if(!GetValidResourcePtr(nResIndex, pResource)) return FALSE;
	return pResource->WriteItemData(szItemPath, pItemData, dwItemSize);
}

#pragma once

#include "IEngineFrame.h"
#include "IEngineRender.h"
#include "IEngineSound.h"
#include "ISysFunction.h"
#include "EngineResource.h"
#include "Animation.h"


class CEngineFrameImp : public IEngineFrame
{
public:
	CEngineFrameImp();
	~CEngineFrameImp();

	virtual void Release();
	virtual BOOL EngineInit(const SEngineInitParam* pEngineInitParam);
	virtual void EngineUpdate();
	virtual BOOL EngineRenderBegin();
	virtual BOOL EngineRenderEnd();
	virtual void EngineGetLastError(LPWSTR szErrorDescription, UINT nErrorSize);
	virtual void EngineSetLastError(LPCWSTR szErrorDescription);
	virtual const SEngineInitParam* GetEngineInitParam() const;

	virtual BOOL DataPackCreate(int& nResIndex, LPCWSTR szPackPath, LPCWSTR szPassword);
	virtual BOOL ImagePackCreate(int& nResIndex, LPCWSTR szItemPath, int nDpkIndex);
	virtual BOOL FontPackCreate(int& nResIndex, LPCWSTR szItemPath, int nDpkIndex);
 	virtual BOOL AnimationCreate(int& nResIndex, LPCWSTR szItemPath, int nDpkIndex);

	virtual BOOL ResourceRelease(int nResIndex);
	virtual BOOL ResourceDraw(int nResIndex, LPCWSTR szText, const SEngineDrawParam* pDrawParam) const;
	virtual BOOL ResourceDraw(int nResIndex, int nItemIndex, const SEngineDrawParam* pDrawParam) const;
 	virtual int  ResourceGetItemCount(int nResIndex) const;
	virtual BOOL ResourceGetItemSize(int nResIndex, int nItemIndex, SIZE& ItemSize) const;
	virtual BOOL ResourceGetItemSize(int nResIndex, LPCWSTR szItemPath, DWORD& dwItemSize) const;
	virtual BOOL ResourceReadItemData(int nResIndex, LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize);
	virtual BOOL ResourceWriteItemData(int nResIndex, LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize);
	virtual BOOL ResourcePlay(int nResIndex, BOOL bLoop);
	virtual BOOL ResourceStop(int nResIndex);


// 	virtual void EngineViewSizeChanged(int nViewCX, int nViewCY);
// 	virtual CSound*		SoundCreate(LPCWSTR szFileName, BOOL bStreaming);
// 	virtual void		SoundPlay(CSound* pSound, BOOL bLoop);

protected:
	BOOL GetValidResourcePtr(int nResIndex, CEngineResource*& pResource) const;
	void AddResourceToVector(int& nResIndex, CEngineResource* pResource);

private:
	IEngineRender*	m_pRender;
	IEngineSound*	m_pSound;
	ISysFile*		m_pFile;

	SEngineInitParam		m_EngineInitParam;
	WCHAR					m_szLastszErrorDescription[MAX_PATH];

	std::vector<CEngineResource*>	m_ResourceVector;

public:
	std::list<CAnimation*>			m_PlayingAnimationList;

// 	std::list<CAnimation*>	m_AnimationList;
// 	std::list<CSound*>		m_SoundList;
// 
// 
// 	typedef std::map<CStringE,CImagePack*>	MapImagePack;
// 	typedef MapImagePack::iterator			MapImagePackIt;
// 	typedef MapImagePack::value_type		MapImagePackType;
// 
// 	CPtrPool<CImagePack>	m_ImagePackPool;
// 	CPool<CFontPack>		m_FontPackPool;
};

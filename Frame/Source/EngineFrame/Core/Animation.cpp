#include "StdAfx.h"
#include "Animation.h"
#include "MemIniFile.h"
#include "EngineFrameImp.h"

#include "MemoryCheck.h"


CAnimation::CAnimation(void)
{
	m_nPlaySingleFrameUpdateCount=0;
	m_nPlayCurrentAniFrame=0;
	m_nAnimationFrameStep=1;
	m_bIsPlaying=FALSE;

	m_pEngine=NULL;
	m_nResIndex=-1;
}

CAnimation::~CAnimation(void)
{
	ASSERT(m_pEngine);
	m_pEngine->ResourceRelease(m_nResIndex);
	m_nResIndex=-1;
}


void CAnimation::Play(BOOL bLoop)
{
	m_nPlaySingleFrameUpdateCount=0;
	m_nPlayCurrentAniFrame=0;
	m_bIsPlaying=TRUE;
	m_bLoop=bLoop;

	std::list<CAnimation*>::const_iterator iter = m_pEngine->m_PlayingAnimationList.begin();
	while (iter != m_pEngine->m_PlayingAnimationList.end())
	{
		if ((*iter) == this)
			break;
		iter++;
	}
	if (iter == m_pEngine->m_PlayingAnimationList.end())
		m_pEngine->m_PlayingAnimationList.push_back(this);
}

void CAnimation::Stop()
{
	m_bIsPlaying=FALSE;
	m_bLoop=FALSE;
	m_pEngine->m_PlayingAnimationList.remove(this);
}

void CAnimation::Update()
{
	if(m_bIsPlaying)
	{
		m_nPlaySingleFrameUpdateCount++;

		if(m_nPlaySingleFrameUpdateCount > m_nAnimationFrameStep)
		{
			m_nPlaySingleFrameUpdateCount=0;
			m_nPlayCurrentAniFrame++;

			ASSERT(m_pEngine);
			int nTotalCount=m_pEngine->ResourceGetItemCount(m_nResIndex);
			if(m_nPlayCurrentAniFrame>=nTotalCount)
			{
				if(m_bLoop)
				{
					m_nPlayCurrentAniFrame=0;
				}
				else
				{
					m_nPlayCurrentAniFrame=nTotalCount-1;
					m_bIsPlaying=FALSE;
				}
			}
		}
	}
}

BOOL CAnimation::Load( LPCWSTR szItemPath, int nDpkIndex, CEngineFrameImp * pEngine )
{
	ASSERT(pEngine);
	m_pEngine=pEngine;
	if(!pEngine->ImagePackCreate(m_nResIndex, szItemPath, nDpkIndex)) return FALSE;

	// 获得ini文件正文
	WCHAR path[MAX_PATH];
	_stprintf_s(path, _T("%s\\Animation.ini"), szItemPath);
	DWORD dwItemSize=0;
	if(!pEngine->ResourceGetItemSize(nDpkIndex, path, dwItemSize)) return FALSE;
	BYTE* pItemData=new BYTE[dwItemSize];
	if(!pEngine->ResourceReadItemData(nDpkIndex, path, pItemData, dwItemSize)) { delete[] pItemData; return FALSE; }

	int n1=pEngine->GetEngineInitParam()->dwUpdateDelay;

	// 读取ini内容
	CMemIniFile ini;
	if(!ini.Load(pItemData, dwItemSize)) { delete[] pItemData; return FALSE; }
	n1=ini.GetValue(_T("Animation"), _T("AniFrameDelayMillisecond"), pEngine->GetEngineInitParam()->dwUpdateDelay);

	delete[] pItemData;

	int n2=pEngine->GetEngineInitParam()->dwUpdateDelay;
	ASSERT(n2);

 	m_nAnimationFrameStep=max(n1/n2,1);

	return TRUE;
}

BOOL CAnimation::Draw( int nItemIndex, const SEngineDrawParam* pDrawParam ) const
{
	ASSERT(-1==nItemIndex);
	ASSERT(m_pEngine);
	return m_pEngine->ResourceDraw(m_nResIndex, m_nPlayCurrentAniFrame, pDrawParam);
}

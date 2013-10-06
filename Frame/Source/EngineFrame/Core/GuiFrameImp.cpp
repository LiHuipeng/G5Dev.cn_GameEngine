#include "stdafx.h"
#include "GuiFrameImp.h"

#include "MemoryCheck.h"


IGuiFrame* GuiFrameCreate( DWORD dwVersion )
{
	if(ENGINEFRAME_VERSION!=dwVersion) return NULL;
	return new CGuiFrameImp;
}


const WCHAR szGuiWndIniPath[]=_T("ini\\GuiWnd.ini");


CGuiFrameImp::CGuiFrameImp()
{
	m_pEngine=NULL;
	m_pMainWnd=NULL;
	m_pCapturedWnd=NULL;
	m_pFocusedWnd=NULL;
	m_bEditorModeOn=FALSE;
	m_nFontIndex=-1;
	m_nDpkIndex=-1;
	m_dwTotalFrames=0;
}

CGuiFrameImp::~CGuiFrameImp()
{
	m_pEngine=NULL;
	m_pMainWnd=NULL;
}

void CGuiFrameImp::Release()
{
	delete this;
}

BOOL CGuiFrameImp::Create( IEngineFrame* pEngine, IGuiWndCore* pMainWnd, int nFontIndex, int nDpkIndex )
{
	ASSERT(nFontIndex>=0);
	m_nFontIndex=nFontIndex;

	ASSERT(nDpkIndex>=0);
	m_nDpkIndex=nDpkIndex;

	ASSERT(pEngine);
	ASSERT(pMainWnd);
	m_pEngine=pEngine;
	m_pMainWnd=pMainWnd;
	m_pFocusedWnd=pMainWnd;

	ASSERT(m_pMainWnd);
	m_pMainWnd->Create(this, pEngine->GetEngineInitParam()->nCanvasCX, pEngine->GetEngineInitParam()->nCanvasCY);

	LoadIni();

	return TRUE;
}

void CGuiFrameImp::Render() const
{
	ASSERT(m_pMainWnd);
	m_pMainWnd->DoRender();

	if(IsEditorModeOn())
	{
		IEngineFrame* pEngine;
		GetEnginePtr(&pEngine);
		ASSERT(pEngine);

		RECT rc={};
		m_pFocusedWnd->GetWindowCanvasRect(&rc);

		// 画控件坐标
		WCHAR s[MAX_PATH]={0};
		_stprintf_s(s, _T("%d,%d"), rc.left, rc.top);

		SEngineDrawParam dp={};
		dp.x=rc.left;
		dp.y=rc.top;
		dp.flag=1;
		dp.color=0xff009f00;
		pEngine->ResourceDraw(m_nFontIndex, s, &dp);
	}
}

void CGuiFrameImp::GetEnginePtr( IEngineFrame** pEngine ) const
{
	*pEngine=m_pEngine;
}


void CGuiFrameImp::Update( const SEngineInputState* pInputState )
{
	m_dwTotalFrames++;

	ASSERT(m_pMainWnd);
	m_pMainWnd->DoUpdate();

	if(m_pCapturedWnd)
	{
		ASSERT(m_pCapturedWnd);
		m_pCapturedWnd->OnCursorEvent(pInputState);
	}
	else
	{
		m_pMainWnd->DoCursorEvent(pInputState);
	}

	if(m_bEditorModeOn && m_pFocusedWnd)
	{
		int mx=0, my=0;
		if(CheckPovClickedEvent(pInputState,   0)) { mx=0; my=-1; }
		if(CheckPovClickedEvent(pInputState,  90)) { mx=1; my=0; }
		if(CheckPovClickedEvent(pInputState, 180)) { mx=0; my=1; }
		if(CheckPovClickedEvent(pInputState, 270)) { mx=-1; my=0; }

		if(mx || my)
		{
			RECT rc;
			ASSERT(m_pFocusedWnd);
			m_pFocusedWnd->GetWindowRect(&rc);
			m_pFocusedWnd->SetWindowPos(rc.left+mx, rc.top+my);
		}
	}
}

void CGuiFrameImp::SwitchEditorMode()
{
	m_bEditorModeOn=!m_bEditorModeOn;

	if(!m_bEditorModeOn)
		SaveIni();
}

void CGuiFrameImp::SetFocus( IGuiWndCore* pWnd )
{
	m_pFocusedWnd=pWnd;
}

void CGuiFrameImp::SetCapture( IGuiWndCore* pWnd )
{
	m_pCapturedWnd=pWnd;
}

BOOL CGuiFrameImp::IsEditorModeOn() const
{
	return m_bEditorModeOn;
}

void CGuiFrameImp::SaveIni()
{
	CMemIniFile ini;
	ini.Load(NULL, 0);

	SaveIniProc(&ini, m_pMainWnd);

	DWORD dwFileSize=ini.GetSaveSize();
	BYTE* pFileData=new BYTE[dwFileSize];
	ZeroMemory(pFileData, dwFileSize);
	ini.Save(pFileData, dwFileSize);

	m_pEngine->ResourceWriteItemData(m_nDpkIndex, szGuiWndIniPath, pFileData, dwFileSize);

	delete[] pFileData;
}

void CGuiFrameImp::SaveIniProc( CMemIniFile* pIniFile, IGuiWndCore* pWnd )
{
	ASSERT(pWnd);

	WCHAR szKey[MAX_PATH]={};
	WCHAR szTmp[MAX_PATH]={};
	IGuiWndCore* p2=pWnd;
	while(true)
	{
		_stprintf_s(szTmp, _T("%d."), p2->GetWindowID());
		p2=p2->GetParent();
		if(!p2) break;
		_tcscat_s(szKey, szTmp);
	}
	_tcscat_s(szKey, _T("M"));
	RECT rc;
	pWnd->GetWindowRect(&rc);
	pIniFile->SetValue(szKey, _T("X"), rc.left);
	pIniFile->SetValue(szKey, _T("Y"), rc.top);

	for(int i=0; i<pWnd->GetChildCount(); i++)
	{
		IGuiWndCore* pChild=pWnd->GetChild(i);
		SaveIniProc(pIniFile, pChild);
	}
}

BOOL CGuiFrameImp::LoadIni()
{
	ASSERT(m_pEngine);

	DWORD dwFileSize=0;
	if(!m_pEngine->ResourceGetItemSize(m_nDpkIndex, szGuiWndIniPath, dwFileSize)) return FALSE;
	BYTE* pFileData=new BYTE[dwFileSize];
	if(!m_pEngine->ResourceReadItemData(m_nDpkIndex, szGuiWndIniPath, pFileData, dwFileSize)) { delete[] pFileData; return FALSE; }

	CMemIniFile ini;
	ini.Load(pFileData, dwFileSize);
	LoadIniProc(&ini, m_pMainWnd);

	delete[] pFileData;
	return TRUE;
}

void CGuiFrameImp::LoadIniProc( CMemIniFile* pIniFile, IGuiWndCore* pWnd )
{
	ASSERT(pWnd);

	WCHAR szKey[MAX_PATH]={};
	WCHAR szTmp[MAX_PATH]={};

	IGuiWndCore* p2=pWnd;
	while(true)
	{
		_stprintf_s(szTmp, _T("%d."), p2->GetWindowID());
		p2=p2->GetParent();
		if(!p2) break;
		_tcscat_s(szKey, szTmp);
	}
	_tcscat_s(szKey, _T("M"));
	int x=pIniFile->GetValue(szKey, _T("X"), 0);
	int y=pIniFile->GetValue(szKey, _T("Y"), 0);
	pWnd->SetWindowPos(x, y);

	for(int i=0; i<pWnd->GetChildCount(); i++)
	{
		IGuiWndCore* pChild=pWnd->GetChild(i);
		LoadIniProc(pIniFile, pChild);
	}
}

DWORD CGuiFrameImp::GetTotalFrames() const
{
	return m_dwTotalFrames;
}

int CGuiFrameImp::GetFontIndex() const
{
	return m_nFontIndex;
}

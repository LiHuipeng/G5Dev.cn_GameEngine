#include "stdafx.h"
#include "AppRes.h"
#include "MainWnd.h"

#include "MacroUtility.h"
#include "MemoryCheck.h"


CGameRes::CGameRes(void)
{
	m_pEngine=EngineFrameCreate(ENGINEFRAME_VERSION);
	ASSERT(m_pEngine);

	m_pGui=GuiFrameCreate(ENGINEFRAME_VERSION);
	ASSERT(m_pGui);

	m_nResourceDpkIndex=-1;
	m_nConfigDpkIndex=-1;
	for(int i=0; i<GAMERES_COUNT; i++)
	{
		m_ResIndex[i]=-1;
	}

	ZeroMemory(&m_InputState, sizeof(m_InputState));
	ZeroMemory(&m_CursorPos, sizeof(m_CursorPos));
	m_bCursorButtonDown=FALSE;
	m_Pov=-1;

	m_nResourceTotalCount=0;
	m_bAllResourceLoadOK=FALSE;
	m_dwTotalFrames=0;
}


CGameRes::~CGameRes(void)
{
}

void CGameRes::Init( HWND hWnd )
{
	SEngineInitParam eip={};
	eip.nCanvasCX=CANVAS_CX;
	eip.nCanvasCY=CANVAS_CY;
	eip.nViewCX=CANVAS_CX;
	eip.nViewCY=CANVAS_CY;
	eip.hAppWnd=hWnd;
	eip.hCanvasWnd=hWnd;
	eip.dwUpdateDelay=GAME_UPDATE_DELAY;
	m_pEngine->EngineInit(&eip);

	m_pEngine->DataPackCreate(m_nResourceDpkIndex, RESOURCE_DPK_PATH, DPK_PASSWORD);
	ASSERT(m_nResourceDpkIndex>=0);
	m_pEngine->DataPackCreate(m_nConfigDpkIndex, CONFIG_DPK_PATH, DPK_PASSWORD);
	ASSERT(m_nConfigDpkIndex>=0);

	ImagePackCreate(IPK_LOADING, _T("Ipk\\Loading"));
}

void CGameRes::Destroy()
{
	ASSERT(m_pEngine);

	for(int i=0; i<GAMERES_COUNT; i++)
	{
		m_pEngine->ResourceRelease(m_ResIndex[i]);
		m_ResIndex[i]=-1;
	}
	m_pEngine->ResourceRelease(m_nResourceDpkIndex);
	m_pEngine->ResourceRelease(m_nConfigDpkIndex);

	SAFE_RELEASE(m_pGui);
	SAFE_RELEASE(m_pEngine);
}

void CGameRes::ImagePackCreate( GAMERES GameRes, LPCWSTR szItemPath )
{
	ASSERT(m_pEngine);
	BOOL b=m_pEngine->ImagePackCreate(m_ResIndex[GameRes], szItemPath, m_nResourceDpkIndex);
	ASSERT(b);
}


void CGameRes::AnimationCreate( GAMERES GameRes, LPCWSTR szItemPath )
{
	ASSERT(m_pEngine);
	BOOL b=m_pEngine->AnimationCreate(m_ResIndex[GameRes], szItemPath, m_nResourceDpkIndex);
	ASSERT(b);
}

void CGameRes::Update()
{
	m_dwTotalFrames++;

	m_InputState.pre=m_InputState.cur;
	m_InputState.cur.CursorPos[0]=m_CursorPos;
	m_InputState.cur.CursorBtn[0]=m_bCursorButtonDown;
	m_InputState.cur.Pov=m_Pov;

	if(m_nResourceTotalCount && m_dwTotalFrames%20==4) // 连续加载资源时由于系统忙，Paint消息会被系统取消。
	{
		int nResCountBeforeLoad=m_ResourceLoadTaskItemQueue.size();
		LoadNextResource();

		if(0==nResCountBeforeLoad && FALSE==m_bAllResourceLoadOK)
		{
			m_nResourceTotalCount=0;
			GuiInit();
			m_bAllResourceLoadOK=TRUE;
		}
	}
	
	if(!m_nResourceTotalCount)
	{
		ASSERT(m_pGui);
		m_pGui->Update(&m_InputState);

		ASSERT(m_pEngine);
		m_pEngine->EngineUpdate();
	}
}

void CGameRes::Draw( LPCWSTR szText, DWORD dwColor, GAMERES GameRes, int x, int y )
{
	ASSERT(m_pEngine);
	SEngineDrawParam dp={};
	dp.x=x;
	dp.y=y;
	dp.w=1000;
	dp.h=100;
	dp.flag = 1;
	dp.color = dwColor;
	BOOL b=m_pEngine->ResourceDraw(m_ResIndex[GameRes], szText, &dp);
	ASSERT(b);
}

void CGameRes::Draw( int nItemIndex, GAMERES GameRes, int x, int y, int w, int h, int sx, int sy, int sw, int sh )
{
	ASSERT(m_ResIndex[GameRes]>=0);
	ASSERT(m_pEngine);
	SEngineDrawParam dp={};
	dp.x=x;
	dp.y=y;
	dp.w=w;
	dp.h=h;
	dp.sx=sx;
	dp.sy=sy;
	dp.sw=sw;
	dp.sh=sh;
	BOOL b=m_pEngine->ResourceDraw(m_ResIndex[GameRes], nItemIndex, &dp);
	ASSERT(b);
}

void CGameRes::Draw( GAMERES GameRes, int x, int y )
{
	ASSERT(m_pEngine);
	SEngineDrawParam dp={};
	dp.x=x;
	dp.y=y;
	BOOL b=m_pEngine->ResourceDraw(m_ResIndex[GameRes], -1, &dp);
	ASSERT(b);
}


void CGameRes::Play( GAMERES GameRes, BOOL bLoop )
{
	ASSERT(m_pEngine);
	BOOL b=m_pEngine->ResourcePlay(m_ResIndex[GameRes], bLoop);
	ASSERT(b);
}

void CGameRes::Stop( GAMERES GameRes )
{
	ASSERT(m_pEngine);
	BOOL b=m_pEngine->ResourceStop(m_ResIndex[GameRes]);
	ASSERT(b);
}

void CGameRes::Render()
{
	ASSERT(m_pEngine);
	m_pEngine->EngineRenderBegin();

	if(m_bAllResourceLoadOK)
	{
		ASSERT(m_pGui);
		m_pGui->Render();
	}
	else
	{
		Draw(0, IPK_LOADING, 0, 0);

		// Loading画面与进度条
		if(m_nResourceTotalCount>0)
		{
			int x=(CANVAS_CX-LOADING_IMAGE_CX)/2;
			int y=CANVAS_CY-LOADING_IMAGE_CY-40;
			int nLoadedCount=m_nResourceTotalCount-m_ResourceLoadTaskItemQueue.size();
			int n=nLoadedCount*LOADING_IMAGE_CX/m_nResourceTotalCount;
			Draw(1, IPK_LOADING, x, y);
			if(n>0)
			{
				if(n>LOADING_IMAGE_CX) n=LOADING_IMAGE_CX;
				Draw(2, IPK_LOADING, x, y, n, LOADING_IMAGE_CY, 0, 0, n, LOADING_IMAGE_CY);
			}
		}
	}

	m_pEngine->EngineRenderEnd();
}

void CGameRes::FontPackCreate( GAMERES GameRes, LPCWSTR szItemPath )
{
	ASSERT(m_pEngine);
	BOOL b=m_pEngine->FontPackCreate(m_ResIndex[GameRes], szItemPath, m_nResourceDpkIndex);
	ASSERT(b);
}

void CGameRes::GuiInit()
{
	m_pGui->Create(m_pEngine, g_MainWnd.m_pCore, GetResIndex(FPK_STANDARD), m_nConfigDpkIndex);
}

int CGameRes::GetResIndex( GAMERES GameRes )
{
	return m_ResIndex[GameRes];
}

void CGameRes::ViewPosToCanvasPos(int ViewX, int ViewY, int &CanvasX, int& CanvasY)
{
	ASSERT(m_pEngine);
	const SEngineInitParam* p=m_pEngine->GetEngineInitParam();
	CanvasX=MulDiv(ViewX, p->nCanvasCX, p->nViewCX);
	CanvasY=MulDiv(ViewY, p->nCanvasCY, p->nViewCY);
}

void CGameRes::MessageProcess( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			int CanvasX, CanvasY;
			ViewPosToCanvasPos(xPos, yPos, CanvasX, CanvasY);
			m_CursorPos.x=CanvasX;
			m_CursorPos.y=CanvasY;
			if(wParam & MK_LBUTTON) m_bCursorButtonDown=TRUE;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			int CanvasX, CanvasY;
			ViewPosToCanvasPos(xPos, yPos, CanvasX, CanvasY);
			m_CursorPos.x=CanvasX;
			m_CursorPos.y=CanvasY;
			m_bCursorButtonDown=TRUE;
		}
		break;

	case WM_LBUTTONUP:
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			int CanvasX, CanvasY;
			ViewPosToCanvasPos(xPos, yPos, CanvasX, CanvasY);
			m_CursorPos.x=CanvasX;
			m_CursorPos.y=CanvasY;
			m_bCursorButtonDown=FALSE;
		}
		break;

	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_UP:    m_Pov=0;   break;
			case VK_RIGHT: m_Pov=90;  break;
			case VK_DOWN:  m_Pov=180; break;
			case VK_LEFT:  m_Pov=270; break;
			}
		}
		break;

	case WM_KEYUP:
		{
			switch (wParam)
			{
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
			case VK_LEFT:
				m_Pov=-1;
			}
		}
		break;

	default:
		break;
	}
}

void CGameRes::AddResourceLoadTask( GAMERES GameRes, LPCWSTR szItemPath )
{
	SResourceLoadTaskItem item;
	item.GameRes=GameRes;
	_tcscpy_s(item.ResourcePath, szItemPath);
	m_ResourceLoadTaskItemQueue.push(item);
	m_nResourceTotalCount++;
}

void CGameRes::LoadNextResource()
{
	if(m_ResourceLoadTaskItemQueue.size()>0)
	{
		SResourceLoadTaskItem item=m_ResourceLoadTaskItemQueue.front();
		m_ResourceLoadTaskItemQueue.pop();

		if(_tcsstr(item.ResourcePath, _T("Ani\\")))
			AnimationCreate(item.GameRes, item.ResourcePath);
		else if(_tcsstr(item.ResourcePath, _T("Fpk\\")))
			FontPackCreate(item.GameRes, item.ResourcePath);
		else
			ImagePackCreate(item.GameRes, item.ResourcePath);
	}
}

void CGameRes::GetResourceLoadProgress( int& nCurrent, int& nTotal )
{
	nCurrent=m_ResourceLoadTaskItemQueue.size();
	nTotal=m_nResourceTotalCount;
}


#include "stdafx.h"
#include "GuiWndCoreImp.h"

#include "MemoryCheck.h"


IGuiWndCore* GuiWndCoreCreate(DWORD dwVersion, IGuiWndShell* pGuiWndShell)
{
	if(ENGINEFRAME_VERSION!=dwVersion) return NULL;
	return new CGuiWndCoreImp(pGuiWndShell);
}


CGuiWndCoreImp::CGuiWndCoreImp(IGuiWndShell* pGuiWndShell) : m_pWndShell(pGuiWndShell)
{
	m_pFrame=NULL;
	m_pParent=NULL;
	m_nResIndex=-1;

	m_bWindowVisible=TRUE;
	m_bWindowEnable=TRUE;
	m_bCursorInThisWindow=FALSE;
	m_bButtonPressDown=FALSE;
	m_bCheckBox=FALSE;
	m_bChecked=FALSE;
}


CGuiWndCoreImp::~CGuiWndCoreImp(void)
{
}

void CGuiWndCoreImp::Release()
{
	delete this;
}

void CGuiWndCoreImp::Create( IGuiFrame* pFrame, int cx, int cy )
{
	m_pFrame=pFrame;

	m_WndRect.left=0;
	m_WndRect.top=0;
	m_WndRect.right=cx;
	m_WndRect.bottom=cy;

	ASSERT(m_pWndShell);
	m_pWndShell->OnCreate();
}

void CGuiWndCoreImp::Create( int nWindowID, IGuiWndCore* pParent, int nResIndex )
{
	ASSERT(!m_pParent); // 防止重复Create

	ASSERT(pParent);
	pParent->AddChild(this);

	m_nWindowID=nWindowID;
	m_pParent=pParent;
	m_pFrame=pParent->GetGuiFrame();
	m_nResIndex=nResIndex;

	IEngineFrame* pEngine;
	ASSERT(m_pFrame);
	m_pFrame->GetEnginePtr(&pEngine);
	ASSERT(pEngine);

	SIZE WndSize={};
	pEngine->ResourceGetItemSize(m_nResIndex, 0, WndSize);
	m_WndRect.left=0;
	m_WndRect.top=0;
	m_WndRect.right=WndSize.cx;
	m_WndRect.bottom=WndSize.cy;

	int nCount=pEngine->ResourceGetItemCount(m_nResIndex);
	if(8==nCount)
		m_bCheckBox=TRUE;

	ASSERT(m_pWndShell);
	m_pWndShell->OnCreate();
}

void CGuiWndCoreImp::DoRender()
{
	IEngineFrame* pEngine;
	ASSERT(m_pFrame);
	m_pFrame->GetEnginePtr(&pEngine);
	ASSERT(pEngine);

	if(IsWindowVisible()==FALSE) return;

	RECT rcCanvas;
	GetWindowCanvasRect(&rcCanvas);

	ASSERT(m_pWndShell);
	m_pWndShell->OnDrawBackground(rcCanvas.left, rcCanvas.top, m_WndRect.right-m_WndRect.left, m_WndRect.bottom-m_WndRect.top, pEngine, m_nResIndex);

	IGuiWndCore* pChild=NULL;
	for(int i=0; i<GetChildCount(); i++)
	{
		pChild=GetChild(i);
		pChild->DoRender();
	}

	ASSERT(m_pWndShell);
	m_pWndShell->OnDrawForeground(rcCanvas.left, rcCanvas.top, m_WndRect.right-m_WndRect.left, m_WndRect.bottom-m_WndRect.top, pEngine, m_nResIndex);
}

BOOL CGuiWndCoreImp::IsWindowVisible() const
{
	return m_bWindowVisible;
}

void CGuiWndCoreImp::GetWindowRect( RECT* pRect ) const
{
	*pRect=m_WndRect;
}

void CGuiWndCoreImp::GetWindowCanvasRect( RECT* pRect ) const
{
	const IGuiWndCore* pParent=GetParent();
	if(pParent)
	{
		RECT rcParent;
		RECT rcThis;
		pParent->GetWindowCanvasRect(&rcParent);

		GetWindowRect(&rcThis);
		OffsetRect(&rcThis, rcParent.left, rcParent.top);
		*pRect=rcThis;
	}
	else
	{
		*pRect=m_WndRect;
	}
}

int CGuiWndCoreImp::GetChildCount() const
{
	return (int)m_Child.size();
}

IGuiWndCore* CGuiWndCoreImp::GetChild( int nIndex ) const
{
	ASSERT(nIndex < GetChildCount());
	return m_Child[nIndex];
}

IGuiWndCore* CGuiWndCoreImp::GetParent() const
{
	return m_pParent;
}

void CGuiWndCoreImp::AddChild( IGuiWndCore* pChildWnd )
{
	m_Child.push_back(pChildWnd);
}

IGuiFrame* CGuiWndCoreImp::GetGuiFrame() const
{
	return m_pFrame;
}

void CGuiWndCoreImp::SetWindowPos( int x, int y )
{
	int dx=x-m_WndRect.left;
	int dy=y-m_WndRect.top;
	OffsetRect(&m_WndRect, dx, dy);
}

void CGuiWndCoreImp::OnCursorEvent( const SEngineInputState* pInputState )
{
	ASSERT(m_pFrame);

	int xCur=pInputState->cur.CursorPos[0].x;
	int yCur=pInputState->cur.CursorPos[0].y;
	int bCur=pInputState->cur.CursorBtn[0];

	int xPre=pInputState->pre.CursorPos[0].x;
	int yPre=pInputState->pre.CursorPos[0].y;
	int bPre=pInputState->pre.CursorBtn[0];

	RECT rcCanvas;
	GetWindowCanvasRect(&rcCanvas);

	POINT pt={xCur, yCur};
	if(PtInRect(&rcCanvas, pt))
	{
		m_bCursorInThisWindow=TRUE;

		ASSERT(m_pFrame);
		if(bCur && !bPre)
			m_pFrame->SetFocus(this);
	}

	// 窗体被 SetCapture 后可能接收到自己矩形之外的鼠标移动或按键通知
	if(bCur && !bPre)
		m_pFrame->SetCapture(this);
	else if(!bCur && bPre)
		m_pFrame->SetCapture(NULL);

	if(m_pParent)
	{
		if(bCur && bPre && m_pFrame->IsEditorModeOn())
		{
			OffsetRect(&m_WndRect, xCur-xPre, yCur-yPre);
		}
	}

	// 触发按钮点击
	{
		ASSERT(IsWindowEnabled());
		ASSERT(m_pFrame);
		if(GetParent() && !m_pFrame->IsEditorModeOn()) // 光标不在矩形内时是不会激发OnCursorEvent函数的，除非按钮被SetCapture。
		{
			if(!bCur && !PtInRect(&rcCanvas, pt))
				m_bButtonPressDown=FALSE;
			else if(!bCur && m_bButtonPressDown)
				DoButtonPress(FALSE);
			else if(bCur && !bPre)
				DoButtonPress(TRUE);
		}
	}
}

BOOL CGuiWndCoreImp::DoCursorEvent( const SEngineInputState* pInputState )
{
	int xCur=pInputState->cur.CursorPos[0].x;
	int yCur=pInputState->cur.CursorPos[0].y;

	BOOL bFocusWndFinded=FALSE;
	IGuiWndCore* pChild=NULL;
	for(int i=GetChildCount()-1; i>=0; i--)
	{
		pChild=GetChild(i);
		if(pChild->IsWindowVisible() && pChild->IsWindowEnabled()) // 如果父窗体不可见，那么无论其子窗体是否可见，都不能让子窗体响应鼠标事件。
			bFocusWndFinded=pChild->DoCursorEvent(pInputState);
		if(bFocusWndFinded) break;
	}

	if(bFocusWndFinded) return TRUE;

	RECT rcCanvas;
	GetWindowCanvasRect(&rcCanvas);
	POINT pt={xCur, yCur};
	if(PtInRect(&rcCanvas, pt) && IsWindowVisible() && IsWindowEnabled())
	{
		OnCursorEvent(pInputState);
		return TRUE;
	}

	return FALSE;
}

BOOL CGuiWndCoreImp::IsWindowEnabled() const
{
	return m_bWindowEnable;
}

void CGuiWndCoreImp::DoButtonPress( BOOL bDown )
{
	if(!IsWindowVisible()) return;
	if(!IsWindowEnabled()) return;

	if(m_bButtonPressDown && !bDown)
	{
		ASSERT(GetParent());
		if(m_bCheckBox)
			m_bChecked=!m_bChecked;
		GetParent()->GetGuiWndShell()->OnButtonClicked(GetWindowID());
	}

	m_bButtonPressDown = bDown;
}

IGuiWndShell* CGuiWndCoreImp::GetGuiWndShell()
{
	return m_pWndShell;
}

int CGuiWndCoreImp::GetWindowID() const
{
	return m_nWindowID;
}

void CGuiWndCoreImp::DrawWindow() const
{
	IEngineFrame* pEngine;
	ASSERT(m_pFrame);
	m_pFrame->GetEnginePtr(&pEngine);
	ASSERT(pEngine);

	if(m_nResIndex>=0)
	{
		RECT rc={};
		GetWindowCanvasRect(&rc);
		SEngineDrawParam dp={};
		dp.x=rc.left;
		dp.y=rc.top;
		pEngine->ResourceDraw(m_nResIndex, 0, &dp);
	}
}

void CGuiWndCoreImp::DrawButton() const
{
	IEngineFrame* pEngine;
	ASSERT(m_pFrame);
	m_pFrame->GetEnginePtr(&pEngine);
	ASSERT(pEngine);

	int n=0;
	if(!IsWindowEnabled())
		n=3;
	else if(m_bButtonPressDown)
		n=2;
	else if(m_bCursorInThisWindow)
		n=1;

	if(m_bCheckBox)
	{
		if(m_bChecked)
			n+=4;
	}

	ASSERT(m_nResIndex>=0);

	RECT rc={};
	GetWindowCanvasRect(&rc);
	SEngineDrawParam dp={};
	dp.x=rc.left;
	dp.y=rc.top;
	pEngine->ResourceDraw(m_nResIndex, n, &dp);
}

void CGuiWndCoreImp::DoUpdate()
{
	OnUpdate();

	IGuiWndCore* pChild=NULL;
	for(int i=0; i<GetChildCount(); i++)
	{
		pChild=GetChild(i);
		pChild->DoUpdate();
	}
}

void CGuiWndCoreImp::OnUpdate()
{
	m_bCursorInThisWindow=FALSE;

	m_pWndShell->OnUpdate();
}

BOOL CGuiWndCoreImp::GetCheck() const
{
	return m_bChecked;
}

void CGuiWndCoreImp::SetCheck( BOOL bCheck )
{
	m_bChecked=bCheck;
}

void CGuiWndCoreImp::DrawEdit() const
{
	IEngineFrame* pEngine;
	ASSERT(m_pFrame);
	m_pFrame->GetEnginePtr(&pEngine);
	ASSERT(pEngine);

	DWORD dwUpdateDelay=pEngine->GetEngineInitParam()->dwUpdateDelay;
	DWORD dwFramesPerSecond=1000 / dwUpdateDelay;

	if(m_nResIndex>=0)
	{
		WCHAR s[MAX_PATH];
		_tcscpy_s(s, _T("Hello World"));
		if(m_pFrame->GetTotalFrames() % dwFramesPerSecond > dwFramesPerSecond/2) _tcscat_s(s, _T("|"));

		int nFontIndex=m_pFrame->GetFontIndex();

		RECT rc={};
		GetWindowCanvasRect(&rc);
		SEngineDrawParam dp={};

		dp.x=rc.left;
		dp.y=rc.top;
		pEngine->ResourceDraw(m_nResIndex, 0, &dp);

		dp.x+=2;
		dp.y+=2;
		dp.flag=1;
		dp.color=0xff000000;
		pEngine->ResourceDraw(nFontIndex, s, &dp);
	}
}

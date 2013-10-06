#include "StdAfx.h"
#include "GuiWnd.h"

#include "MacroUtility.h"
#include "MemoryCheck.h"


CGuiWnd::CGuiWnd()
{
	m_pCore=GuiWndCoreCreate(ENGINEFRAME_VERSION, this);
}

CGuiWnd::~CGuiWnd()
{
	SAFE_RELEASE(m_pCore);
}


void CGuiWnd::OnCreate()
{

}


void CGuiWnd::OnDrawForeground( int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex ) const
{
}

void CGuiWnd::Create( int nWindowID, IGuiWndCore* pParent, int nResIndex )
{
	ASSERT(m_pCore);
	m_pCore->Create(nWindowID, pParent, nResIndex);
}

void CGuiWnd::OnDrawBackground( int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex ) const
{
	ASSERT(m_pCore);
	m_pCore->DrawWindow();
}

void CGuiWnd::SetWindowPos( int x, int y )
{
	ASSERT(m_pCore);
	m_pCore->SetWindowPos(x, y);
}

void CGuiWnd::OnButtonClicked( int nWindowID )
{

}

void CGuiWnd::OnUpdate()
{

}

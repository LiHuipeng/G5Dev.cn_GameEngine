#include "StdAfx.h"
#include "GuiButton.h"

#include "MemoryCheck.h"


CGuiButton::CGuiButton(void)
{
}

CGuiButton::~CGuiButton(void)
{

}

void CGuiButton::OnDrawBackground( int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex ) const
{
	ASSERT(m_pCore);
	m_pCore->DrawButton();
}


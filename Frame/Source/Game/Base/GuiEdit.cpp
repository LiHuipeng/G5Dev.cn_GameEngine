#include "StdAfx.h"
#include "GuiEdit.h"

#include "MemoryCheck.h"


CGuiEdit::CGuiEdit(void)
{
}

CGuiEdit::~CGuiEdit(void)
{

}

void CGuiEdit::OnDrawBackground( int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex ) const
{
	ASSERT(m_pCore);
	m_pCore->DrawEdit();
}


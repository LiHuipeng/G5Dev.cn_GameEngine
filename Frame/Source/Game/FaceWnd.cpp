#include "stdafx.h"
#include "FaceWnd.h"
#include "GameApp.h"


CFaceWnd::CFaceWnd(void)
{
	m_bShowFace=TRUE;
}


CFaceWnd::~CFaceWnd(void)
{
}

void CFaceWnd::OnCreate()
{
	m_OKBtn.Create(GUI_OK_BTN, m_pCore, g_Game.m_Res.GetResIndex(IPK_OK_BTN));
	m_CancelBtn.Create(GUI_CANCEL_BTN, m_pCore, g_Game.m_Res.GetResIndex(IPK_CANCEL_BTN));
	m_FaceEdit.Create(GUI_FACE_EDIT, m_pCore, g_Game.m_Res.GetResIndex(IPK_FACE_EDIT));
}

void CFaceWnd::OnDrawBackground( int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex ) const
{
	__super::OnDrawBackground(CanvasX, CanvasY, WindowCX, WindowCY, pEngine, nResIndex);

	if(m_bShowFace)
	{
		int x=24+CanvasX;
		int y=15+CanvasY;
		int x0=x;
		for(int i=0; i<8; i++)
		{
			g_Game.m_Res.Draw(i, IPK_FACE, x, y);
			x+=64+5;

			if(i%4==3)
			{
				x=x0;
				y+=64+5;
			}
		}
	}
}

void CFaceWnd::OnButtonClicked( int nWindowID )
{
	__super::OnButtonClicked(nWindowID);

	switch (nWindowID)
	{
	case GUI_OK_BTN:
		m_bShowFace=TRUE;
		break;

	case GUI_CANCEL_BTN:
		m_bShowFace=FALSE;
		break;
	}
}

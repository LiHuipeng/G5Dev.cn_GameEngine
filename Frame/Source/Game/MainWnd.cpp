#include "stdafx.h"
#include "MainWnd.h"
#include "GameApp.h"

#include "MemoryCheck.h"


CMainWnd g_MainWnd;


CMainWnd::CMainWnd(void)
{
}


CMainWnd::~CMainWnd(void)
{
}

void CMainWnd::OnDrawBackground( int CanvasX, int CanvasY, int WindowCX, int WindowCY, IEngineFrame* pEngine, int nResIndex ) const
{
	int x=6, y=6;
	for(int i=0; i<12; i++)
	{
		g_Game.m_Res.Draw(i, IPK_FACE, x, y);
		x+=70;
	}

	g_Game.m_Res.Draw(ANI_STAR_1, 100, 200);
	g_Game.m_Res.Draw(ANI_STAR_2, 500, 200);

	x=10; y=600;
	g_Game.m_Res.Draw(_T("按【P】播放动画，按【S】停止。"), 0xff007700, FPK_STANDARD, x, y);
	g_Game.m_Res.Draw(_T("按【E】切换窗口编辑模式。"), 0xff007700, FPK_STANDARD, x, y+30);
}

void CMainWnd::OnCreate()
{
	m_FaceWnd.Create(GUI_FACE_WND, m_pCore, g_Game.m_Res.GetResIndex(IPK_FACE_WND));

	g_Game.m_Res.Play(ANI_STAR_1, TRUE);
	g_Game.m_Res.Play(ANI_STAR_2);
}

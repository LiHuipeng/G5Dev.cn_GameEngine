#include "stdafx.h"
#include "GameApp.h"
#include "MacroUtility.h"


CGame g_Game;


CGame::CGame(void)
{
}


void CGame::Init( HWND hWnd )
{
	m_Res.Init(hWnd);

	m_Res.AddResourceLoadTask(FPK_STANDARD, _T("Fpk\\ËÎÌו25"));

	m_Res.AddResourceLoadTask(IPK_FACE, _T("Ipk\\Face"));

	m_Res.AddResourceLoadTask(ANI_STAR_1, _T("Ani\\Star"));
	m_Res.AddResourceLoadTask(ANI_STAR_2, _T("Ani\\Star"));

	m_Res.AddResourceLoadTask(IPK_FACE_WND, _T("Gui\\FaceWnd"));
	m_Res.AddResourceLoadTask(IPK_OK_BTN, _T("Gui\\OKBtn"));
	m_Res.AddResourceLoadTask(IPK_CANCEL_BTN, _T("Gui\\CancelBtn"));
	m_Res.AddResourceLoadTask(IPK_FACE_EDIT, _T("Gui\\FaceEdit"));

// 	bool b=false;
// 	while(true)
// 	{
// 		b=m_Res.LoadNextResource();
// 		if(b) break;
// 	}
}

void CGame::Destroy()
{
	m_Res.Destroy();
}

void CGame::Update()
{
	m_Res.Update();
}

void CGame::Render()
{
	m_Res.Render();
}

void CGame::OnKeyDown( WPARAM wParam, LPARAM lParam )
{
	switch (wParam)
	{
	case _T('P'):
		m_Res.Play(ANI_STAR_2);
		break;
	case _T('S'):
		m_Res.Stop(ANI_STAR_2);
		break;
	case _T('E'):
		m_Res.m_pGui->SwitchEditorMode();
	}
}


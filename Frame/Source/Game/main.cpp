// main.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GameApp.h"
#include <mmsystem.h>
#include "MemoryCheck.h"


#pragma comment(lib, "winmm.lib")

#define GAME_UPDATE_DELAY		20

HWND		g_hAppWnd=NULL;
MMRESULT	g_hTimeEvent=NULL;

void CALLBACK TimeSetEventProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	PostMessage(g_hAppWnd, WM_TIMER, 0, 0);
}



#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	EnableMemLeakCheck(); // 调用了这句后，即使cpp文件中没有插入MemoryCheck.h，开发环境也能发现内存泄露，但看不到行号。

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 主消息循环:
	while (msg.message != WM_QUIT) 
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_GAME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	DWORD dwWndStyle=0x96ca0000;

	// 计算WindowRect
	RECT rcClient={0,0,CANVAS_CX,CANVAS_CY};
	RECT rcWindow=rcClient;
	::AdjustWindowRectEx(&rcWindow, dwWndStyle, FALSE, NULL);

	// 计算AppWnd的位置和尺寸
	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,SPIF_SENDCHANGE);
	SIZE AppWndSize={0,0};
	POINT AppWndPos={0,0};
	AppWndSize.cx=rcWindow.right-rcWindow.left;
	AppWndSize.cy=rcWindow.bottom-rcWindow.top;
	AppWndPos.x=((rcWorkArea.right-rcWorkArea.left)-AppWndSize.cx)/2;
	AppWndPos.y=((rcWorkArea.bottom-rcWorkArea.top)-AppWndSize.cy)/2;

	hWnd = CreateWindow(szWindowClass, szTitle, dwWndStyle,
		AppWndPos.x, AppWndPos.y, AppWndSize.cx, AppWndSize.cy, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	g_hAppWnd=hWnd;


	g_Game.Init(g_hAppWnd);

	// game update timer
	g_hTimeEvent = timeSetEvent(GAME_UPDATE_DELAY, GAME_UPDATE_DELAY, TimeSetEventProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION);


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_Game.m_Res.MessageProcess(message, wParam, lParam);

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			static BOOL bRenderProcessing=FALSE;
			if(!bRenderProcessing) // 防止海海的ASSERT
			{
				bRenderProcessing=TRUE;
				g_Game.Render();
				bRenderProcessing=FALSE;
			}

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		{
			g_Game.Destroy();
			PostQuitMessage(0);
		}
		break;
	case WM_TIMER:
		{
			static BOOL bUpdateProcessing=FALSE;
			if(!bUpdateProcessing) // 防止海海的ASSERT
			{
				bUpdateProcessing=TRUE;

				{
					g_Game.Update();

					RECT rcClient;
					::GetClientRect(g_hAppWnd, &rcClient);
					::InvalidateRect(g_hAppWnd, &rcClient, FALSE);
				}

				bUpdateProcessing=FALSE;
			}
		}
		break;
	case WM_KEYDOWN:
		{
			g_Game.OnKeyDown(wParam, lParam);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

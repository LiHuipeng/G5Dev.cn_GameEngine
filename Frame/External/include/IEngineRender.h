// color 格式 0xaarrggbb;
//

#pragma once


#define ENGINE_FONT_FACE_NAME_ARLEN		50		// Array Length


struct IEngineImage
{
	virtual int		GetWidth() const = 0;
	virtual int		GetHeight() const = 0;
	virtual void	Draw(LONG x, LONG y, LONG w, LONG h, LONG sx, LONG sy, LONG sw, LONG sh, DWORD color) const = 0;
	virtual BOOL	DrawToFillRect(const RECT* pRect) const = 0;			// 用当前图片以重复的方式铺满整个矩形
	virtual BOOL	FillBitsToThis(LPCVOID Bits, DWORD Length) = 0;			// 用二进制池中的数据来填充图片
	virtual void	Release() = 0;
};

__interface IEngineRender
{
	virtual void Release() = 0;

	virtual BOOL Init(HWND hCanvasWnd, int nCanvasCX, int nCanvasCY, int nViewCX, int nViewCY) = 0;
	virtual BOOL RenderBegin() = 0;
	virtual BOOL RenderEnd() = 0;
	virtual IEngineImage* CreateImage(LPCVOID pData, DWORD dwSize, DWORD ColorKey) = 0;
};


#ifdef RENDER_EXPORTS
#define RENDER_API __declspec(dllexport)
#else
#define RENDER_API __declspec(dllimport)
#endif

#define ENGINE_RENDER_VERSION	20130916 //20110106

RENDER_API IEngineRender* EngineRenderCreate();

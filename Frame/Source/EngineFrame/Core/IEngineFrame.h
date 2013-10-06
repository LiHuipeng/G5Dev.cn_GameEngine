// huipeng_lph@163.com modify at 2013-09-23

#pragma once


#define  ENGINEFRAME_VERSION	20130923


struct SEngineInitParam // 引擎初始化参数
{
	HWND	hAppWnd;
	HWND	hCanvasWnd;
	int		nCanvasCX;
	int		nCanvasCY;
	int		nViewCX;
	int		nViewCY;
	DWORD	dwUpdateDelay;			// 相邻两渲染帧之间的时间间隔(毫秒)
};

struct SEngineDrawParam
{
	LONG x;
	LONG y;
	LONG w;
	LONG h;
	LONG sx;
	LONG sy;
	LONG sw;
	LONG sh;
	DWORD color;
	DWORD flag;				// 如果需要混色，flag值就一定不为0。
};


__interface IEngineFrame
{
	virtual void Release() = 0;

	virtual BOOL EngineInit(const SEngineInitParam* pEngineInit) = 0;
	virtual void EngineUpdate() = 0;
	virtual BOOL EngineRenderBegin() = 0;
	virtual BOOL EngineRenderEnd() = 0;
	virtual void EngineGetLastError(LPWSTR szErrorDescription, UINT nErrorSize) = 0;
	virtual void EngineSetLastError(LPCWSTR szErrorDescription) = 0;
	virtual const SEngineInitParam* GetEngineInitParam() const = 0;

	virtual BOOL DataPackCreate(int& nResIndex, LPCWSTR szPackPath, LPCWSTR szPassword) = 0;
	virtual BOOL ImagePackCreate(int& nResIndex, LPCWSTR szItemPath, int nDpkIndex) = 0;
	virtual BOOL FontPackCreate(int& nResIndex, LPCWSTR szItemPath, int nDpkIndex) = 0;
	virtual BOOL AnimationCreate(int& nResIndex, LPCWSTR szItemPath, int nDpkIndex) = 0;

	virtual BOOL ResourceRelease(int nResIndex) = 0;
	virtual BOOL ResourceDraw(int nResIndex, LPCWSTR szText, const SEngineDrawParam* pDrawParam) const = 0;
	virtual BOOL ResourceDraw(int nResIndex, int nItemIndex, const SEngineDrawParam* pDrawParam) const = 0;
	virtual int  ResourceGetItemCount(int nResIndex) const = 0;
	virtual BOOL ResourceGetItemSize(int nResIndex, int nItemIndex, SIZE& ItemSize) const = 0;
	virtual BOOL ResourceGetItemSize(int nResIndex, LPCWSTR szItemPath, DWORD& dwItemSize) const = 0;
	virtual BOOL ResourceReadItemData(int nResIndex, LPCWSTR szItemPath, LPVOID pItemData, DWORD dwItemSize) = 0;
	virtual BOOL ResourceWriteItemData(int nResIndex, LPCWSTR szItemPath, LPCVOID pItemData, DWORD dwItemSize) = 0;
	virtual BOOL ResourcePlay(int nResIndex, BOOL bLoop);
	virtual BOOL ResourceStop(int nResIndex);
};


#ifdef ENGINEFRAME_EXPORTS
#define ENGINEFRAME_API __declspec(dllexport)
#else
#define ENGINEFRAME_API __declspec(dllimport)
#endif


ENGINEFRAME_API IEngineFrame* EngineFrameCreate(DWORD dwVersion);


/*

__interface

Visual Studio 2010

Microsoft Specific

A Visual C++ interface can be defined as follows: 

Can inherit from zero or more base interfaces.
Cannot inherit from a base class.
Can only contain public, pure virtual methods.
Cannot contain constructors, destructors, or operators.
Cannot contain static methods.
Cannot contain data members; properties are allowed.

*/

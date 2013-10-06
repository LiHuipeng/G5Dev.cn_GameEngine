// 2013-09-04 huipeng_lhp@163.com
// ISysFunction接口不单用在客户端，它也可用在服务端。
// 所以它的内部不可以写与客户端绑定的转义实现方法。
#pragma once


struct SSysDatetime {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;	// Sunday : wDayOfWeek==0
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};

__interface ISysFunction
{
	virtual void Release() = 0;
	virtual int  GetSystemMetricsScreenCX() = 0;
	virtual int  GetSystemMetricsScreenCY() = 0;
	virtual BOOL GetSystemProductName(LPWSTR szProductName, DWORD dwStringLength) = 0;
	virtual void GetSystemDatetime(SSysDatetime* datetime) = 0;
	virtual BOOL GetLastEthernetAdapterMAC(LPWSTR szMac, int nMacLength) = 0;
	virtual BOOL GetLocalHostIP( LPWSTR szIP, int nIPLength ) = 0;
	virtual BOOL StringUtf8ToUnicode(WCHAR* szUnicode, DWORD dwUnicodeLength, const char* szUtf8) = 0;
	virtual BOOL StringUnicodeToUtf8(char* szUtf8, DWORD dwUtf8Length, const WCHAR* szUnicode) = 0;
};


__interface ISysFile
{
	virtual void  Release() = 0;
	virtual BOOL  Open(LPCWSTR szFilePath, BOOL bForRead) = 0;
	virtual void  Close() = 0;
	virtual BOOL  Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, DWORD* lpNumberOfBytesRead) = 0;
	virtual BOOL  Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, DWORD* lpNumberOfBytesWritten) = 0;
	virtual DWORD Seek(DWORD dwIndex) = 0;
	virtual DWORD Size() const = 0;
	virtual BOOL IsFileExists(LPCWSTR szFilePath) = 0;
};


#define SYSFUNCTION_VERSION		20130918 //20130904

#ifdef WIN32
    #ifdef SYSFUNCTION_EXPORTS
    #define SYSFUNCTION_API __declspec(dllexport)
    #else
    #define SYSFUNCTION_API __declspec(dllimport)
    #endif
#else
    #define SYSFUNCTION_API
#endif


SYSFUNCTION_API ISysFunction* SysFunctionCreate(DWORD dwVersion);
SYSFUNCTION_API ISysFile*     SysFileCreate(DWORD dwVersion);

#pragma once


#ifndef _WINDEF_

typedef void* HANDLE;

struct HWND__;
typedef HWND__* HWND;

struct HDC__;
typedef HDC__* HDC;

#endif

typedef const wchar_t *LPCWSTR, *PCWSTR;
typedef const char *LPCSTR, *PCSTR;

#ifdef  UNICODE                     // r_winnt

typedef LPCWSTR PCTSTR, LPCTSTR;

#else   /* UNICODE */               // r_winnt

typedef LPCSTR LPCTSTR;

#endif

typedef HANDLE       PDH_HCOUNTER;
typedef HANDLE       PDH_HQUERY;


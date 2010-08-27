#ifndef LSACTIVEDESKTOP_H_
#define LSACTIVEDESKTOP_H_

#include "./lsapi.h"

extern "C"
{
	__declspec(dllexport) int __cdecl initModuleEx(HWND parent, HINSTANCE dll, LPCSTR szPath);
	__declspec(dllexport) void __cdecl quitModule(HINSTANCE dllInst);
}

#endif

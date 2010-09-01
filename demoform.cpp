#define STRICT
#define NOCRYPT

#include <windows.h>
#include "lsadsettings.h"
#include "webform.h"
#include "demoform.h"
#include <tchar.h>
#include <list>
#include "webwindow.h"

HINSTANCE hInstance;
HWND hMain;         // Our main window

const TCHAR* className = _T("classLSActiveDesktop");
LPCSTR revID = "LSActiveDesktop 0.1 by Tobbe";

bool loaded;
LSADSettings settings;
std::list<WebWindow*> webWindows;

void __cdecl bangNavigate(HWND caller, const char* args);
void reportError(LPCSTR msg);
void readSettings();
LRESULT CALLBACK PlainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

LRESULT CALLBACK PlainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case LM_GETREVID:
			lstrcpy((LPSTR)lParam, revID);

			return strlen((LPTSTR)lParam);
		case LM_REFRESH:
			readSettings();

			return 0;
		case WM_CREATE:
			int x = 100;
			int y = 100;
			for (std::list<WebWindow*>::iterator it = webWindows.begin(); it != webWindows.end(); it++) {
				(*it)->Create(hInstance, x, y, 1300, 300);
				y += 400;
			}
			break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

extern "C" int __cdecl initModuleEx(HWND parentWnd, HINSTANCE dllInst, LPCSTR szPath)
{
	hInstance = dllInst;
	OleInitialize(0);

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)PlainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex)) {
		reportError("Error registering LSActiveDesktop window class");
		return 1;
	}

	readSettings();

	webWindows.push_back(new WebWindow());
	webWindows.push_back(new WebWindow());

	hMain = CreateWindowEx(0, className, _T("WindowLSActiveDesktop"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		1000, 700, 200, 200, NULL, NULL, hInstance, NULL);

	if (hMain == NULL) {
		reportError("Error creating LSActiveDesktop window");
		UnregisterClass(className, dllInst);
		return 1;
	}

	ShowWindow(hMain, SW_SHOW);

	// Register our bangs with LiteStep
	AddBangCommand("!LSActiveDesktopNavigate", bangNavigate);
	/*AddBangCommand("!LSActiveDesktopBack", bangBack);
	AddBangCommand("!LSActiveDesktopForward", bangForward);
	AddBangCommand("!LSActiveDesktopRefresh", bangRefresh);
	AddBangCommand("!LSActiveDesktopRefreshCache", bangRefreshCache);*/

	// Register message for version info
	UINT msgs[] = {LM_GETREVID, LM_REFRESH, 0};
	SendMessage(GetLitestepWnd(), LM_REGISTERMESSAGE, (WPARAM)hMain, (LPARAM)msgs);

	return 0;
}

void __cdecl bangNavigate(HWND caller, const char* args)
{
	if (args && args[0] != '\0') {
		SetWindowText(webWindows.front()->webForm->hWnd, args);
	}
}

void readSettings()
{
	settings.showErrors = GetRCBoolDef("LSActiveDesktopShowErrors", TRUE) != FALSE;
	settings.showScrollbars = GetRCBoolDef("LSActiveDesktopShowScrollBars", TRUE) != FALSE;
	/*settings.unmuteOnVolUp = GetRCBoolDef("tVolEzyUnmuteOnVolUp", TRUE) != FALSE;
	settings.unmuteOnVolDown = GetRCBoolDef("tVolEzyUnmuteOnVolDown", FALSE) != FALSE;
	char buffer[1024];
	GetRCString("tVolEzyVolumeChangedCommand", buffer, "!none", 1024);
	settings.volumeChangedCommand = buffer;
	GetRCString("tVolEzyMuteChangedCommand", buffer, "!none", 1024);
	settings.muteChangedCommand = buffer;*/
}

void reportError(LPCSTR msg)
{
	if (settings.showErrors)
	{
		MessageBox(NULL, msg, _T("LSActiveDesktop error"), MB_OK | MB_ICONERROR);
	}
}

extern "C" void __cdecl quitModule(HINSTANCE dllInst)
{
	RemoveBangCommand("!LSActiveDesktopNavigate");
	/*RemoveBangCommand("!LSActiveDesktopBack");
	RemoveBangCommand("!LSActiveDesktopForward");
	RemoveBangCommand("!LSActiveDesktopRefresh");
	RemoveBangCommand("!LSActiveDesktopRefreshCache");*/

	UINT msgs[] = {LM_GETREVID, LM_REFRESH, 0};
	SendMessage(GetLitestepWnd(), LM_UNREGISTERMESSAGE, (WPARAM)hMain, (LPARAM)msgs);

	for (std::list<WebWindow*>::iterator it = webWindows.begin(); it != webWindows.end(); it++) {
		delete *it;
	}

	if (hMain != NULL)
	{
		DestroyWindow(hMain);
		hMain = NULL;
	}

	UnregisterClass(className, dllInst);
	OleUninitialize();
}

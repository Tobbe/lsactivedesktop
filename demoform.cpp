#define STRICT
#define NOCRYPT

#include <windows.h>
#include "lsadsettings.h"
#include "webform.h"
#include "demoform.h"
#include <tchar.h>

HINSTANCE hInstance;
HWND hMain;         // Our main window

const TCHAR* className = _T("classLSActiveDesktop");
LPCSTR revID = "LSActiveDesktop 0.1 by Tobbe";

bool loaded;
LSADSettings settings;
TWebf *webForm;

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
			webForm->create(hwnd, hInstance, 103, settings.showScrollbars);

			break;
		case WM_SIZE:
			if (webForm->hWnd) {
				MoveWindow(webForm->hWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
			}

			break;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			FillRect(ps.hdc, &ps.rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));
			EndPaint(hwnd,&ps);

			return 0;
		}
		case WM_COMMAND: {
			int id = LOWORD(wParam);
			int code = HIWORD(wParam);

			if (id == 103 && code == WEBFN_LOADED) {
				loaded = true;
			}

			break;
		}
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

	webForm = new TWebf();

	hMain = CreateWindowEx(0, className, _T("WindowLSActiveDesktop"), WS_POPUP | WS_CLIPCHILDREN,
		200, 300, 1300, 600, NULL, NULL, hInstance, NULL);

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
		SetWindowText(webForm->hWnd, args);
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

	if (hMain != NULL)
	{
		DestroyWindow(hMain);
		hMain = NULL;
	}

	UnregisterClass(className, dllInst);
	OleUninitialize();
}

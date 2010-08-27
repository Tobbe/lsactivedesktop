#define STRICT
#define NOCRYPT

#include <windows.h>
#include "lsadsettings.h"
#include "webform.h"
#include "demoform.h"

HINSTANCE hInstance;
HWND hMain;         // Our main window
HWND hwebf;         // We declare this handle globally, just for convenience

const TCHAR* className = _T("classLSActiveDesktop");
LPCSTR revID = "LSActiveDesktop 0.1 by Tobbe";

bool loaded;
LSADSettings settings;

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
			hwebf = CreateWindow(
				WEBFORM_CLASS,
				_T("http://tlundberg.com"),
				WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
				0, 0, 100, 100, hwnd, (HMENU)103, hInstance, 0);

				MessageBox(NULL, "Just created the webform", "WM_CREATE", MB_OK);

			break;
		case WM_SIZE:
			MoveWindow(hwebf, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);

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
		/*case WM_DESTROY:
			PostQuitMessage(0);

			break;*/
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/*extern "C" int __cdecl initModuleEx(HWND parentWnd, HINSTANCE dllInst, LPCSTR szPath)
{
	OleInitialize(0);
	hWndParent = parentWnd;
	hInstance = dllInst;

	readSettings();

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.lpfnWndProc = (WNDPROC)wndProc;
	wc.hInstance = dllInst;
	wc.lpszClassName = className;
	wc.style = CS_NOCLOSE;
	if (!RegisterClass(&wc))
	{
		reportError("Error registering tVolEzy window class");
		return 1;
	}

	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, className, "", WS_CHILD,
		0, 0, 0, 0, hWndParent, NULL, dllInst, NULL);
	if (hWnd == NULL)
	{
		reportError("Error creating tVolEzy window");
		UnregisterClass(className, dllInst);
		return 1;
	}

	// Register our bangs with LiteStep
	AddBangCommand("!LSActiveDesktopNavigate", bangNavigate);
	AddBangCommand("!LSActiveDesktopBack", bangBack);
	AddBangCommand("!LSActiveDesktopForward", bangForward);
	AddBangCommand("!LSActiveDesktopRefresh", bangRefresh);
	AddBangCommand("!LSActiveDesktopRefreshCache", bangRefreshCache);

	// Register message for version info
	UINT msgs[] = {LM_GETREVID, LM_REFRESH, 0};
	SendMessage(GetLitestepWnd(), LM_REGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)msgs);

	return 0;
}*/

//int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int)
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
		//MessageBox(NULL, _T("Failed to register class"), _T("Error"), MB_OK);
		reportError("Error registering LSActiveDesktop window class");
		return 1;
	}

	readSettings();

	HWND parent = NULL;

	if (parentWnd != NULL && parentWnd != GetLitestepWnd()) {
		parent = parentWnd;
	}

	hMain = CreateWindowEx(0, className, _T("WindowLSActiveDesktop"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, GetLitestepWnd(), NULL, hInstance, NULL);

	if (hMain == NULL) {
		/*MessageBox(NULL, _T("Failed to create window"), _T("Error"), MB_OK);
		return 0;*/
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
/*
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	OleUninitialize();
	return (int)msg.wParam;*/

	MessageBox(NULL, "End of initModuleEx", "initModuleEx", MB_OK);
}

void __cdecl bangNavigate(HWND caller, const char* args)
{
	if (args && args[0] != '\0') {
		SetWindowText(hwebf, args);
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

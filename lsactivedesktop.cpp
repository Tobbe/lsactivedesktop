#define STRICT
#define NOCRYPT

#include <windows.h>
#include "lsadsettings.h"
#include "webform.h"
#include "lsactivedesktop.h"
#include <tchar.h>
#include <map>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "webwindow.h"
#include "webformdispatchimpl.h"
#include "jslitestep.h"
#include "lsad.h"

HINSTANCE hInstance;
HWND hMain;         // Our main window
HWND hWndLSAD;      // Window to send messages to for communication
                    // with the web forms

const TCHAR* className = _T("classLSActiveDesktop");
LPCSTR revID = "LSActiveDesktop 0.4 by Tobbe";

bool loaded;
LSADSettings settings;
LSAD *lsad = NULL;

void __cdecl bangHandler(HWND caller, const char* bangCommandName, const char* args);
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
		case WM_LSADCREATED:
			hWndLSAD = (HWND)wParam;
			AddBangCommandEx("!LSActiveDesktopNavigate", bangHandler);
			AddBangCommandEx("!LSActiveDesktopRunJSFunction", bangHandler);
			AddBangCommandEx("!LSActiveDesktopForward", bangHandler);
			AddBangCommandEx("!LSActiveDesktopBack", bangHandler);
			AddBangCommandEx("!LSActiveDesktopRefresh", bangHandler);
			AddBangCommandEx("!LSActiveDesktopRefreshCache", bangHandler);
			break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

extern "C" int __cdecl initModuleEx(HWND parentWnd, HINSTANCE dllInst, LPCSTR szPath)
{
	hInstance = dllInst;

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

	hMain = CreateWindowEx(0, className, _T("WindowLSActiveDesktop"), 0, 0, 0,
		0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

	if (hMain == NULL) {
		reportError("Error creating LSActiveDesktop window");
		UnregisterClass(className, dllInst);
		return 1;
	}

	// Register message for version info
	UINT msgs[] = {LM_GETREVID, LM_REFRESH, 0};
	SendMessage(GetLitestepWnd(), LM_REGISTERMESSAGE, (WPARAM)hMain, (LPARAM)msgs);

	lsad = new LSAD(hInstance, hMain, &settings);

	DWORD tID;
	HANDLE hThread = CreateThread(NULL, 0, LSAD::ThreadEntry, lsad, 0, &tID);

	return 0;
}

void __cdecl bangHandler(HWND caller, const char* bangCommandName, const char* args)
{
	const char *tokenStart = args;
	char token[MAX_LINE_LENGTH + 1];
	if (!GetToken(tokenStart, token, &tokenStart, false)) {
		reportError("Wrong bang command syntax");
	}
	std::string name(token);
	std::transform(name.begin(), name.end(), name.begin(), tolower);
	WPARAM wParamName = reinterpret_cast<WPARAM>(new std::string(name));
	LPARAM lParamArgs = 0L;

	UINT msg = 0;

	std::string bangName = bangCommandName;
	std::transform(bangName.begin(), bangName.end(), bangName.begin(), tolower);

	if (bangName == "!lsactivedesktopnavigate") {
		if (!GetToken(tokenStart, token, &tokenStart, false)) {
			reportError("Wrong bang command syntax");
			delete reinterpret_cast<std::string*>(wParamName);
			return;
		}
		std::string url(token);

		lParamArgs = reinterpret_cast<LPARAM>(new std::string(url));
		msg = LSAD_BANGNAVIGATE;
	} else if (bangName == "!lsactivedesktoprunjsfunction") {
		if (!tokenStart) {
			reportError("Wrong bang command syntax");
			delete reinterpret_cast<std::string*>(wParamName);
			return;
		}

		std::string cmd(tokenStart);

		lParamArgs = reinterpret_cast<LPARAM>(new std::string(cmd));
		msg = LSAD_BANGRUNJSFUNCTION;
	} else if (bangName == "!lsactivedesktopforward") {
		msg = LSAD_BANGFORWARD;
	} else if (bangName == "!lsactivedesktopback") {
		msg = LSAD_BANGBACK;
	} else if (bangName == "!lsactivedesktoprefresh") {
		msg = LSAD_BANGREFRESH;
	} else if (bangName == "!lsactivedesktoprefreshcache") {
		msg = LSAD_BANGREFRESHCACHE;
	}

	PostMessage(hWndLSAD, msg, wParamName, lParamArgs);
}

void readSettings()
{
	settings.showErrors = GetRCBoolDef("LSActiveDesktopShowErrors", TRUE) != FALSE;

	char line[MAX_LINE_LENGTH + 1];
	const char *tokenStart = line;
	char token[MAX_LINE_LENGTH + 1];
	GetRCLine("LSActiveDesktopWebWindows", line, MAX_LINE_LENGTH + 1, NULL);
	while (GetToken(tokenStart, token, &tokenStart, false)) {
		std::string name(token);
		std::transform(name.begin(), name.end(), name.begin(), tolower);

		LSADWebWndProp props;
		props.x = GetRCCoordinate((name + "X").c_str(), 0, GetSystemMetrics(SM_CXVIRTUALSCREEN));
		props.y = GetRCCoordinate((name + "Y").c_str(), 0, GetSystemMetrics(SM_CYVIRTUALSCREEN));
		props.width = GetRCInt((name + "Width").c_str(), 100);
		props.height = GetRCInt((name + "Height").c_str(), 100);
		props.showScrollbars = !(GetRCBool((name + "HideScrollbars").c_str(), TRUE) != FALSE);

		char url[MAX_LINE_LENGTH + 1];
		GetRCString((name + "URL").c_str(), url, "http://tlundberg.com", MAX_LINE_LENGTH + 1);
		props.url = url;

		settings.windowProperties.insert(make_pair(name, props));
	}
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
	RemoveBangCommand("!LSActiveDesktopRunJSFunction");
	RemoveBangCommand("!LSActiveDesktopBack");
	RemoveBangCommand("!LSActiveDesktopForward");
	RemoveBangCommand("!LSActiveDesktopRefresh");
	RemoveBangCommand("!LSActiveDesktopRefreshCache");

	UINT msgs[] = {LM_GETREVID, LM_REFRESH, 0};
	SendMessage(GetLitestepWnd(), LM_UNREGISTERMESSAGE, (WPARAM)hMain, (LPARAM)msgs);

	delete lsad;

	if (hMain != NULL)
	{
		DestroyWindow(hMain);
		hMain = NULL;
	}

	UnregisterClass(className, dllInst);
}

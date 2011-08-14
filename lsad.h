#ifndef _LSAD_H__
#define _LSAD_H__

#include <windows.h>
#include <map>
#include <string>

#define LSAD_BANGNAVIGATE WM_APP + 1
#define LSAD_BANGBACK WM_APP + 2
#define LSAD_BANGFORWARD WM_APP + 3
#define LSAD_BANGRUNJSFUNCTION WM_APP + 4
#define LSAD_BANGREFRESH WM_APP + 5
#define LSAD_BANGREFRESHCACHE WM_APP + 6
#define WM_LSADCREATED WM_APP + 150

class WebWindow;
class JSLiteStep;
class WebformDispatchImpl;
struct LSADSettings;

class LSAD {
private:
	HINSTANCE hInstance;
	HWND hWnd;
	HWND hWndLSMsgWnd;
	std::map<std::string, WebWindow*> webWindows;
	JSLiteStep *jsLiteStep;
	WebformDispatchImpl *webformDispatchImpl;
	LSADSettings *settings;

public:
	LSAD(HINSTANCE hInstance, HWND hWndLSMsgWnd, LSADSettings *settings);
	static DWORD WINAPI ThreadEntry(LPVOID lpParameter);
	DWORD ThreadMain();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT InstanceWndProc(UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif

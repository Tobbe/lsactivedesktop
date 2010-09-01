#ifndef _WEB_WINDOW_H__
#define _WEB_WINDOW_H__

#include <windows.h>
#include "webform.h"

class WebWindow {
private:
	HWND hWndWebWindow;
	HINSTANCE hInstWebWindow;
	static LRESULT CALLBACK WebWindowWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT InstanceWndProc(UINT msg, WPARAM wParam, LPARAM lParam);
public:
	TWebf *webForm;
	void Create(HINSTANCE hInstance, UINT x, UINT y, UINT width, UINT height);
};

#endif

#include "lsad.h"
#include "webform.h"
#include "lsapi.h"
#include "webwindow.h"
#include "webformdispatchimpl.h"
#include "jslitestep.h"
#include "lsadsettings.h"
#include <string>

static const char className[] = "LSADWndClass";

LSAD::LSAD(HINSTANCE hInstance, HWND hWndLSMsgWnd, LSADSettings *settings)
{
	this->hInstance = hInstance;
	this->hWndLSMsgWnd = hWndLSMsgWnd;
	this->settings = settings;
}

DWORD LSAD::ThreadEntry(LPVOID lpParameter)
{
	LSAD *that = static_cast<LSAD*>(lpParameter);
	return that->ThreadMain();
}

DWORD LSAD::ThreadMain()
{
	OleInitialize(0);

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(this);
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, "Failed to register lsad class", "Error", MB_OK);
		//reportError("Error registering LSActiveDesktop window class");
		return 1;
	}

	jsLiteStep = new JSLiteStep();
	jsLiteStep->AddRef();
	webformDispatchImpl = new WebformDispatchImpl(jsLiteStep);

	for (std::map<std::string, LSADWebWndProp>::iterator it = settings->windowProperties.begin(); it != settings->windowProperties.end(); it++) {
		webWindows.insert(std::make_pair(it->first, new WebWindow(webformDispatchImpl)));
	}

	HWND hWndCreate = CreateWindowEx(0, className, "LSAD", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, (LPVOID)this);

	if (hWndCreate == NULL) {
		MessageBox(NULL, "Failed to create window", "Error", MB_OK);
		//reportError("Error creating LSActiveDesktop window");
		UnregisterClass(className, hInstance);
		return 1;
	}

	PostMessage(hWndLSMsgWnd, WM_LSADCREATED, (WPARAM)hWndCreate, (LPARAM)NULL);

	//ShowWindow(hWndCreate, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for (std::map<std::string, WebWindow*>::iterator it = webWindows.begin(); it != webWindows.end(); it++) {
		delete it->second;
	}

	delete webformDispatchImpl;
	jsLiteStep->Release();

	OleUninitialize();
	return (int)msg.wParam;
}

LRESULT CALLBACK LSAD::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE) {
		LSAD *lsad = (LSAD*)((LPCREATESTRUCT(lParam))->lpCreateParams);
		lsad->hWnd = hWnd;

		#pragma warning(suppress:4244)
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)lsad);

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	#pragma warning(suppress:4312)
	LSAD *lsad = (LSAD*)GetWindowLongPtr(hWnd, 0);

	if (lsad == NULL) {
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return lsad->InstanceWndProc(msg, wParam, lParam);
}

LRESULT LSAD::InstanceWndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE: {
			for (std::map<std::string, WebWindow*>::iterator it = webWindows.begin(); it != webWindows.end(); it++) {
				int x = settings->windowProperties[it->first].x;
				int y = settings->windowProperties[it->first].y;
				int width = settings->windowProperties[it->first].width;
				int height = settings->windowProperties[it->first].height;
				std::string url = settings->windowProperties[it->first].url;
				bool showScrollbars = settings->windowProperties[it->first].showScrollbars;
				it->second->Create(hInstance, x, y, width, height, showScrollbars);
				it->second->webForm->Go(url.c_str());
				y += 400;
			}

			break;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			FillRect(ps.hdc, &ps.rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));
			EndPaint(hWnd,&ps);

			return 0;
		}
		case LSAD_BANGBACK: {
			std::string *name = reinterpret_cast<std::string*>(wParam);
			webWindows[*name]->webForm->Back();
			delete name;

			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);

			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

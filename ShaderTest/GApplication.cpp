#include "GApplication.h"
#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4244)

#define MSG_OUT(MSG) std::cout << MSG << std::endl;

extern bool WinCreate(HWND hwnd);
extern void WinLoop();
extern void WinDestroy();

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

GApplication* GApplication::s_pApplication = nullptr;

GApplication::GApplication( const GString& name, int x, int y, int cx, int cy )
{
	s_pApplication = this;
	WNDCLASSEXA wnd_class = { 0 };
	wnd_class.cbSize = sizeof(wnd_class);
	wnd_class.hCursor = LoadCursorA(GetModuleHandleA(nullptr), IDC_ARROW);
	wnd_class.hIcon = LoadIconA(GetModuleHandleA(nullptr), IDC_ICON);
	wnd_class.hInstance = GetModuleHandleA(nullptr);
	wnd_class.lpfnWndProc = WndProc;
	wnd_class.lpszClassName = name.c_str();
	wnd_class.style = CS_VREDRAW | CS_HREDRAW;

	LRESULT result = ::RegisterClassExA(&wnd_class);
	if(FAILED(result))
		MSG_OUT("RegisterClassExA error!");


	m_hWnd = CreateWindowExA(
			0,
			name.c_str(),
			name.c_str(),
			WS_OVERLAPPEDWINDOW,
			x, y, cx, cy,
			nullptr, nullptr,
			GetModuleHandleA(nullptr),
			nullptr
		);
	if(m_hWnd == nullptr)
		MSG_OUT("CreateWindowExA error!");


	if (!WinCreate(m_hWnd))
		DestroyWindow();
	::UnregisterClassA(name.c_str(), GetModuleHandleA(nullptr));

	result = ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	if(FAILED(result))
		MSG_OUT("ShowWindow error!");
	result = ::UpdateWindow(m_hWnd);
	if(FAILED(result))
		MSG_OUT("UpdateWindow error!");





	m_strName = name;
	RECT clientRect = { 0 };
	::GetClientRect(m_hWnd, &clientRect);
	m_ClientSize.width = clientRect.right - clientRect.left;
	m_ClientSize.height = clientRect.bottom - clientRect.top;
}

GApplication::~GApplication()
{
	s_pApplication = nullptr;
	DestroyWindow();
}

int GApplication::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if(PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		else
			WinLoop();
	}

	return msg.wParam;
}

void GApplication::DestroyWindow()
{
	if(m_hWnd)
		::DestroyWindow(m_hWnd);
}

void GApplication::Exit()
{
	PostQuitMessage(0);
}

void GApplication::RegisterProcess( void* instance, process_t func )
{
	GWINPROCESS process;
	process.instance = (GWinProcessor*)instance;
	process.callback = func;
	m_WinProcessVector.push_back(process);
}

void GApplication::UnregisterProcess( void* instance )
{
	auto itor = m_WinProcessVector.begin();
	for (; itor != m_WinProcessVector.end(); ++itor)
		if((*itor).instance == instance)
			m_WinProcessVector.erase(itor);
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	GApplication* app = GApplication::s_pApplication;
	auto itor = app->m_WinProcessVector.begin();
	for (; itor != app->m_WinProcessVector.end(); ++itor)
	{
		GWINPROCESS& process = *itor;
		if((process.instance->*process.callback)(hwnd, message, wParam, lParam))
			return 0;
	}


	switch (message)
	{
	case WM_DESTROY:
		WinDestroy();
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProcA(hwnd, message, wParam, lParam);
	}

	return 0;
}

GApplication* dxGetApp()
{
	return GApplication::s_pApplication;
}


#pragma warning(pop)
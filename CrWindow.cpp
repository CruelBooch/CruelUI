#include "CrWindow.h"
#include "CrWidgets.h"
#include "CruelUI.h"
#include "CrGraphics.h"


#include <chrono>
#include <Windows.h>

namespace CruelUI 
{
	//全局变量
	WNDCLASSEX								CruelUIMainWndClass = { NULL };			//窗口类
	int										WndCount = 0;							//窗口计数
	bool									isInit = false;							//是否初始化
	std::unordered_map<HWND, Window*>		CrWindows;								//窗口列表
	CruelUIStyle Dark, Light, Theme;

	void LaunchWindow(std::wstring title, int width, int height, int nCmdShow, Window* wnd)
	{
		// 初始化
		if (!isInit)
		{
			// 注册窗口类
			CruelUIMainWndClass.cbSize = sizeof(WNDCLASSEX);
			CruelUIMainWndClass.lpfnWndProc = CrWindowProc;
			CruelUIMainWndClass.lpszClassName = L"CruelUI";
			CruelUIMainWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			CruelUIMainWndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			if (CruelUIMainWndClass.hInstance == NULL)
				CruelUIMainWndClass.hInstance = GetModuleHandle(NULL);

			RegisterClassEx(&CruelUIMainWndClass);

			isInit = true;

			InitColor();

			// 初始化D2D	
			InitD2D();
		}

		// 创建窗口
		wnd->hWnd = CreateWindowEx(
			0,
			CruelUIMainWndClass.lpszClassName,
			(wchar_t*)title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			NULL, NULL, CruelUIMainWndClass.hInstance, NULL
		);		

		if (wnd->hWnd == NULL)
		{
			Log(L"Create window failed", LERROR);
			return;
		}
		
		Log(L"Window is Ready", LINFO);
		wnd->isAlive = true;

		// 将窗口添加到列表
		CrWindows.emplace(wnd->hWnd, wnd);

		// 显示
		ShowWindow(wnd->hWnd, SW_SHOW);

		// 渲染线程
		std::thread RenderThread(Render, wnd);	
		
		RenderThread.detach();

		// 计数
		WndCount++;

		// 消息循环
		MSG msg = { };
		while (true)
		{
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// 处理消息
			wnd->OnMessage(msg);

			Sleep(10);
		}

		wnd->isAlive = false;

		// 释放资源
		SafeRelease(&wnd->pRenderTarget);
		Log(std::wstring(L"[INFO] Window Exit @") + wnd->WindowTitle, LINFO);
	}

	void Log(const std::wstring& content, int type)
	{
#ifdef _DEBUG
		std::wstring types[] = {
			L"[Info]:\t",
			L"[Warning]:\t",
			L"[Error]:\t"
		};
		std::wcout << types[type] << content << std::endl;
#endif
	}

	void Sleepfor(int ms)
	{
		static auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		float delayTime = ms/1000.0f;

		if (deltaTime < delayTime) {
			std::this_thread::sleep_for(
				std::chrono::milliseconds(static_cast<int>((delayTime - deltaTime) * 1000))
			);
		}
	}

	void InitColor()
	{
		//Dark Theme
		Dark.Colors[CrColor_Text] = COLORF(0xF0F0F0, 1.0f);
		Dark.Colors[CrColor_Text] = COLORF(0xE0E0E0, 1.0f);

		Dark.Colors[CrColor_Button] = COLORF(0x4285F4, 1.0f);
		Dark.Colors[CrColor_ButtonHovered] = COLORF(0x8AB4F8, 1.0f);
		Dark.Colors[CrColor_ButtonActived] = COLORF(0x669DF6, 1.0f);
		Dark.Colors[CrColor_ButtonDisabled] = COLORF(0x3A4A6B, 1.0f);

		Dark.Colors[CrColor_WndBg] = COLORF(0x1f1f1f, 1.0f);

		//Light Theme
		Light.Colors[CrColor_Text] = COLORF(0x212121, 1.0f);
		Light.Colors[CrColor_SecondaryText] = COLORF(0x424242, 1.0f);

		Light.Colors[CrColor_Button] = COLORF(0x4285F4, 1.0f);
		Light.Colors[CrColor_ButtonHovered] = COLORF(0x3367D6, 1.0f);
		Light.Colors[CrColor_ButtonActived] = COLORF(0x2549B8, 1.0f);
		Light.Colors[CrColor_ButtonDisabled] = COLORF(0x9EBAF8, 1.0f);

		Light.Colors[CrColor_WndBg] = COLORF(0xF0F0F0, 1.0f);


		Theme = Dark;
	}

	void SetColorDark()
	{
		Theme = Dark;
	}

	void SetColorLight()
	{
		Theme = Light;
	}

	void Render(Window* wnd)
	{
		// 初始化D2D渲染目标
		RECT rc;
		GetClientRect(wnd->hWnd, &rc);
		pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				wnd->hWnd,
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top),
				D2D1_PRESENT_OPTIONS_IMMEDIATELY
			),
			&wnd->pRenderTarget
		);

		//获取窗口标题
		std::wstring WndTitle = wnd->WindowTitle;
		Log(std::wstring(L"Render Thread Joined @") + WndTitle, LINFO);

		while (wnd->isAlive)
		{
			wnd->pRenderTarget->Resize(D2D1::SizeU(wnd->GetClientRect().right - wnd->GetClientRect().left, wnd->GetClientRect().bottom - wnd->GetClientRect().top));

			wnd->pRenderTarget->BeginDraw();

			wnd->pRenderTarget->Clear(Theme.Colors[CrColor_WndBg]);
			
			if (!wnd->Widgets.empty())
				for (auto& widget : wnd->Widgets)
					widget.second->OnPaint();

			wnd->pRenderTarget->EndDraw();

			Sleepfor(1000 / 30);
		}

		Log(std::wstring(L"Render Thread Exited @") + WndTitle, LINFO);
	}

	void SetAppIcon(HICON hIcon, HICON hIconSm)
	{
		if (hIcon != NULL)
			CruelUIMainWndClass.hIcon = hIcon;
		if (hIconSm != NULL)
			CruelUIMainWndClass.hIconSm = hIconSm;
	}

	void SetAppHInstance(HINSTANCE hInstance)
	{
		if (hInstance != NULL)
			CruelUIMainWndClass.hInstance = hInstance;
		else
			CruelUIMainWndClass.hInstance = GetModuleHandle(NULL);
	}

	LRESULT CrWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{	
		Window* wnd = NULL;	// 窗口对象;

		// 获取窗口对象
		if (CrWindows.find(hwnd) != CrWindows.end())
			wnd = CrWindows[hwnd];

		switch (uMsg)
		{
		case WM_SIZE:
		{
			if (wnd == NULL)
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_PAINT:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	Window::Window()
	{
	}

	Window::Window(const wchar_t* window_title, int width, int height, int _nCmdShow)
		: WindowTitle(window_title), Width(width), Height(height), ShowMode(_nCmdShow)
	{
	}

	void Window::Create()
	{
		std::thread(LaunchWindow, WindowTitle, Width, Height, ShowMode, this).detach();

	}
	 
	void Window::Show(int nCmdShow) const
	{
		nCmdShow = nCmdShow;
		ShowWindow(hWnd, nCmdShow);
	}

	std::wstring Window::GetTitle()
	{
		return WindowTitle;
	}

	RECT Window::GetClientRect()
	{
		RECT rect;
		::GetClientRect(hWnd, &rect);
		return rect;
	}

	HWND Window::GetHandle() const
	{
		return hWnd;
	}

	ID2D1HwndRenderTarget* Window::GetpRT()
	{
		return pRenderTarget;
	}

	bool Window::IsAlive() const
	{
		return isAlive;
	}

	void Window::SetTitle(const wchar_t* title) const
	{
		SetWindowTextW(hWnd, title);
	}

	void Window::CloseWindow()
	{
		if (hWnd)
		{
			DestroyWindow(hWnd);
			hWnd = NULL;
		}
	}

	int Window::AddWidget(WidgetPtr widget)
	{
		if (!widget) 
			return -1;

		int id = WidgetCounter++;
		Widgets.emplace(id, widget);  // 使用emplace插入
		return id;
	}

	void Window::RemoveWidget(WidgetPtr widget)
	{
		Widgets.erase(widget->GetID());
	}

	void Window::OnMessage(MSG msg)
	{
		for (auto& widget : Widgets)
		{
			widget.second->UpdateState(msg);
			widget.second->Event();
		}
	}

	Window::~Window()
	{
		CrWindows.erase(hWnd);
		WndCount--;

		SafeRelease(&pRenderTarget);
	}
}

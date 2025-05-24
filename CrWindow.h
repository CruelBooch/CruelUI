#pragma once

#include <windows.h>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "CrGraphics.h"

#include "CruelUI.h"

#include "CrWidgets.h"

#define LINFO		0
#define LWARNING	1
#define LERROR		2

namespace CruelUI {
	//前向声明
	class Widget;
	class Window;
	class Graphics;

	//LOG
	void Log(const std::wstring& content, int type);
	//休眠
	void Sleepfor(int ms);

	void InitColor();
	void SetColorDark();
	void SetColorLight();

	// 设置应用程序图标
	// 注意：该函数须在注册所有窗口之前调用
	void SetAppIcon(HICON hIcon, HICON hIconSm);

	// 设置应用程序实例句柄
	// 注意：该函数须在注册所有窗口之前调用
	void SetAppHInstance(HINSTANCE hInstance);
	void LaunchWindow(std::wstring title, int width, int height, int nCmdShow, Window* wnd);
	void Render(Window* wnd);
	LRESULT CALLBACK CrWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	template<class T>
	void SafeRelease(T** ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = nullptr;
		}
	}

	// 窗口类
	class Window {
	public:
		using WidgetPtr = std::shared_ptr<Widget>;
		using WidgetMap = std::unordered_map<int, WidgetPtr>;

		Window();
		Window(const Window& c) = delete;
		Window(const wchar_t* window_title, int width, int height, int _nCmdShow);

		// 创建并启动窗口
		// !阻塞
		friend void LaunchWindow(std::wstring title, int width, int height, int nCmdShow, Window* wnd);
		// 默认窗口过程函数
		friend LRESULT CALLBACK CrWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		// 渲染函数
		friend void Render(Window* wnd);

		// 创建窗口
		void Create();

		// 显示窗口
		void Show(int nCmdShow) const;

		// 获取标题
		std::wstring GetTitle();
		// 获取客户端矩形
		RECT GetClientRect();
		// 获取窗口句柄
		HWND GetHandle() const;
		// 获取D2D渲染目标
		ID2D1HwndRenderTarget* GetpRT();
		// 获取画布指针
		Graphics* GetpGraphics() { return &MainGraphics; }
		// 窗口是否存在
		bool IsAlive() const;
		// 设置窗口标题
		void SetTitle(const wchar_t* title) const;
		// 关闭窗口
		void CloseWindow();


		// 注册控件[工厂]
		template<typename T, typename... Args>
		std::shared_ptr<T> CreateWidget(Args&&... args) {
			// 检查T是否派生自Widget
			static_assert(std::is_base_of_v<Widget, T>, "T must derive from Widget");
			auto widget = std::make_shared<T>(std::forward<Args>(args)..., this);
			widget->ID = AddWidget(widget);
			return widget;
		}
		//添加/删除控件
		int AddWidget(WidgetPtr widget);
		void RemoveWidget(WidgetPtr widget);

		// 过程
		void OnMessage(MSG msg);

		~Window();
	protected:		
		HWND hWnd;										// 窗口句柄
		std::wstring WindowTitle;						// 窗口标题
		int Width;										// 窗口宽度
		int Height;										// 窗口高度
		int ShowMode;									// 窗口显示方式
		int	WidgetCounter = 0;							// 控件数量
		bool isAlive;									// 窗口是否存在
		WidgetMap Widgets = {};							// 窗口控件
		ID2D1HwndRenderTarget* pRenderTarget = nullptr;	// D2D主渲染目标
		Graphics MainGraphics;							// 画布
	};
}
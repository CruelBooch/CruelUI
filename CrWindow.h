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
	//ǰ������
	class Widget;
	class Window;
	class Graphics;

	//LOG
	void Log(const std::wstring& content, int type);
	//����
	void Sleepfor(int ms);

	void InitColor();
	void SetColorDark();
	void SetColorLight();

	// ����Ӧ�ó���ͼ��
	// ע�⣺�ú�������ע�����д���֮ǰ����
	void SetAppIcon(HICON hIcon, HICON hIconSm);

	// ����Ӧ�ó���ʵ�����
	// ע�⣺�ú�������ע�����д���֮ǰ����
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

	// ������
	class Window {
	public:
		using WidgetPtr = std::shared_ptr<Widget>;
		using WidgetMap = std::unordered_map<int, WidgetPtr>;

		Window();
		Window(const Window& c) = delete;
		Window(const wchar_t* window_title, int width, int height, int _nCmdShow);

		// ��������������
		// !����
		friend void LaunchWindow(std::wstring title, int width, int height, int nCmdShow, Window* wnd);
		// Ĭ�ϴ��ڹ��̺���
		friend LRESULT CALLBACK CrWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		// ��Ⱦ����
		friend void Render(Window* wnd);

		// ��������
		void Create();

		// ��ʾ����
		void Show(int nCmdShow) const;

		// ��ȡ����
		std::wstring GetTitle();
		// ��ȡ�ͻ��˾���
		RECT GetClientRect();
		// ��ȡ���ھ��
		HWND GetHandle() const;
		// ��ȡD2D��ȾĿ��
		ID2D1HwndRenderTarget* GetpRT();
		// ��ȡ����ָ��
		Graphics* GetpGraphics() { return &MainGraphics; }
		// �����Ƿ����
		bool IsAlive() const;
		// ���ô��ڱ���
		void SetTitle(const wchar_t* title) const;
		// �رմ���
		void CloseWindow();


		// ע��ؼ�[����]
		template<typename T, typename... Args>
		std::shared_ptr<T> CreateWidget(Args&&... args) {
			// ���T�Ƿ�������Widget
			static_assert(std::is_base_of_v<Widget, T>, "T must derive from Widget");
			auto widget = std::make_shared<T>(std::forward<Args>(args)..., this);
			widget->ID = AddWidget(widget);
			return widget;
		}
		//���/ɾ���ؼ�
		int AddWidget(WidgetPtr widget);
		void RemoveWidget(WidgetPtr widget);

		// ����
		void OnMessage(MSG msg);

		~Window();
	protected:		
		HWND hWnd;										// ���ھ��
		std::wstring WindowTitle;						// ���ڱ���
		int Width;										// ���ڿ��
		int Height;										// ���ڸ߶�
		int ShowMode;									// ������ʾ��ʽ
		int	WidgetCounter = 0;							// �ؼ�����
		bool isAlive;									// �����Ƿ����
		WidgetMap Widgets = {};							// ���ڿؼ�
		ID2D1HwndRenderTarget* pRenderTarget = nullptr;	// D2D����ȾĿ��
		Graphics MainGraphics;							// ����
	};
}
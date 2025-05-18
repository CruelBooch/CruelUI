#pragma once

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


#include <string>
#include <unordered_map>
#include <functional>
#include <memory>


namespace CruelUI
{
	//前向声明
	typedef D2D1_RECT_F RECTF;
	typedef D2D1_POINT_2F POINTF;
	typedef D2D1::ColorF COLORF;
	class Window;

	enum ElmentState
	{
		Normal,
		Hovered,
		Actived,
		LBClick,
		RBClick,
		MBClick,
		Disabled,
	};

	struct DragState
	{
		bool IsDrag = false;			//是否拖动
		POINTF Pos;					//起始点
	};

	struct BORDER
	{
		float Left;
		float Top;
		float Right;
		float Bottom;
	};

	//触发器
	class Trigger
	{
	public:
		//构造函数
		Trigger(BORDER border, RECTF rect, Window* parent);

		//获取触发器状态
		ElmentState	GetState() const;

		//更新触发器状态
		virtual ElmentState UpdateState(MSG msg);

		//设置触发器位置
		void SetRect(RECTF new_rect);
	protected:
		BORDER Border;					//边距
		RECTF Rect;						//位置（四个点的坐标）
		ElmentState State = Normal;		//状态
		ElmentState OldState = Normal;	//上一次状态
		Window* Parent;					//父窗口
	};

	class Widget : public Trigger
	{
	public:
		Widget(std::wstring content, BORDER border, RECTF rect, Window* parent);
		~Widget();

		//获取控件ID
		int GetID() const { return ID; }
		//获取控件内容
		std::wstring GetContent() const { return Content; }

		//控件事件
		virtual void Event() = 0;

		//绘制
		virtual void OnPaint() = 0;

		int ID;					//控件ID
	protected:
		std::wstring Content;	//控件内容

	};

	class Button : public Widget
	{
	public:
		using ClickHandler = std::function<void()>;
		
		Button(std::wstring content, RECTF rect, float radius, ClickHandler handler, Window* parent);
		Button(std::wstring content, BORDER border, RECTF rect, float radius, ClickHandler handler, Window* parent);
		~Button();

		//绘制
		void OnPaint() override;
		//设置按钮内容
		void SetContent(std::wstring content) { Content = content; }
		//设置按钮点击事件
		void SetOnClick(ClickHandler handler);
		//禁用、启用
		bool IsEnable();
		void Disable();
		void Enable();
		//事件
		void Event() override;

	protected:
		float Radius = 5.0f;	// 圆角半径
		bool isEnable = true;	// 是否启用
		ClickHandler OnClick;	// 触发事件
	};

}
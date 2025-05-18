#include "CrWidgets.h"
#include "CrDef.h"
#include "CrWindow.h"
#include "CruelUI.h"

namespace CruelUI
{
	Trigger::Trigger(BORDER border, RECTF rect, Window* parent)
		: Border(border), Rect(rect), Parent(parent)
	{
	}

	ElmentState Trigger::GetState() const
	{
		return State;
	}


	ElmentState Trigger::UpdateState(MSG msg)
	{
		//获取鼠标坐标
		POINT cursor_point;
		GetCursorPos(&cursor_point);
		ScreenToClient(Parent->GetHandle(), &cursor_point);

		std::wstring state[] = {
			L"Normal",
			L"Hovered",
			L"Actived",
			L"LBClick",
			L"RBClick",
			L"MBClick",
			L"Disabled",
		};

		//Log(state[State], 0);

		if (cursor_point.x >= Rect.left && cursor_point.y >= Rect.top && cursor_point.x <= Rect.right && cursor_point.y <= Rect.bottom)
		{
			State = Hovered;
			if (msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN || msg.message == WM_MBUTTONDOWN)
			{
				State = Actived;
			}
			if (OldState == Actived)
			{
				switch (msg.message)
				{
				case WM_LBUTTONUP:
					State = LBClick;
					break;
				case WM_RBUTTONUP:
					State = RBClick;
					break;
				case WM_MBUTTONUP:
					State = MBClick;
					break;
				}
			}
		}
		else
			State = Normal;

		OldState = State;

		return State;
	}

	void Trigger::SetRect(RECTF new_rect)
	{
		Rect = new_rect;
	}

	Widget::Widget(std::wstring content, BORDER border, RECTF rect, Window* parent)
		: Content(content), Trigger(border, rect, parent)
	{
	}
	Widget::~Widget()
	{
	}

	Button::Button(std::wstring content, RECTF rect, float radius, ClickHandler handler, Window* parent)
		: Widget(content, {0}, rect, parent), Radius(radius), OnClick(handler)
	{
	}

	Button::Button(std::wstring content, BORDER border, RECTF rect, float radius, ClickHandler handler, Window* parent)
		: Widget(content, border, rect, parent), Radius(radius), OnClick(handler)
	{
	}

	Button::~Button()
	{
	}

	//绘制按钮
	void Button::OnPaint()
	{
		ID2D1SolidColorBrush* pFillBrush = NULL;
		ID2D1SolidColorBrush* pBorderBrush = NULL;
		ID2D1SolidColorBrush* pTextBrush = nullptr;
		IDWriteTextFormat* pTextFormat = nullptr;

		D2D1::ColorF border_color(1, 0, 1, 1);
		D2D1::ColorF fill_color(1, 0, 1, 1);

		switch (State)
		{
		case Hovered:
			border_color = Theme.Colors[CrColor_ButtonHovered];
			fill_color = Theme.Colors[CrColor_ButtonHovered];
			break;
		case LBClick:
		case RBClick:
		case MBClick:
		case Actived:
			border_color = Theme.Colors[CrColor_ButtonActived];
			fill_color = Theme.Colors[CrColor_ButtonActived];
			break;
		case Disabled:
			border_color = Theme.Colors[CrColor_ButtonDisabled];
			fill_color = Theme.Colors[CrColor_ButtonDisabled];
		default:
			border_color = Theme.Colors[CrColor_Button];
			fill_color = Theme.Colors[CrColor_Button];
		}

		Parent->GetpRT()->CreateSolidColorBrush(
			border_color,
			&pBorderBrush
		);
		Parent->GetpRT()->CreateSolidColorBrush(
			fill_color,
			&pFillBrush
		);
		Parent->GetpRT()->FillRoundedRectangle(
			{ Rect, Radius, Radius },
			pFillBrush
			);
		Parent->GetpRT()->DrawRoundedRectangle(
			{ Rect, Radius, Radius },
			pBorderBrush,
			0.5f
			);

		// 释放资源
		SafeRelease(&pFillBrush);
		SafeRelease(&pBorderBrush);
	}

	//设置回调函数
	void Button::SetOnClick(ClickHandler handler)
	{
		OnClick = handler;
	}

	bool Button::IsEnable()
	{
		return isEnable;
	}

	void Button::Disable()
	{
		State = Disabled;
		isEnable = false;
	}

	void Button::Enable()
	{
		State = Normal;
		isEnable = true;
	}

	void Button::Event()
	{
		if (isEnable == false)
			return;
		if (Trigger::GetState() == LBClick)
		{
			OnClick();
		}
	}
}
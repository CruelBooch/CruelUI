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
	//ǰ������
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
		bool IsDrag = false;			//�Ƿ��϶�
		POINTF Pos;					//��ʼ��
	};

	struct BORDER
	{
		float Left;
		float Top;
		float Right;
		float Bottom;
	};

	//������
	class Trigger
	{
	public:
		//���캯��
		Trigger(BORDER border, RECTF rect, Window* parent);

		//��ȡ������״̬
		ElmentState	GetState() const;

		//���´�����״̬
		virtual ElmentState UpdateState(MSG msg);

		//���ô�����λ��
		void SetRect(RECTF new_rect);
	protected:
		BORDER Border;					//�߾�
		RECTF Rect;						//λ�ã��ĸ�������꣩
		ElmentState State = Normal;		//״̬
		ElmentState OldState = Normal;	//��һ��״̬
		Window* Parent;					//������
	};

	class Widget : public Trigger
	{
	public:
		Widget(std::wstring content, BORDER border, RECTF rect, Window* parent);
		~Widget();

		//��ȡ�ؼ�ID
		int GetID() const { return ID; }
		//��ȡ�ؼ�����
		std::wstring GetContent() const { return Content; }

		//�ؼ��¼�
		virtual void Event() = 0;

		//����
		virtual void OnPaint() = 0;

		int ID;					//�ؼ�ID
	protected:
		std::wstring Content;	//�ؼ�����

	};

	class Button : public Widget
	{
	public:
		using ClickHandler = std::function<void()>;
		
		Button(std::wstring content, RECTF rect, float radius, ClickHandler handler, Window* parent);
		Button(std::wstring content, BORDER border, RECTF rect, float radius, ClickHandler handler, Window* parent);
		~Button();

		//����
		void OnPaint() override;
		//���ð�ť����
		void SetContent(std::wstring content) { Content = content; }
		//���ð�ť����¼�
		void SetOnClick(ClickHandler handler);
		//���á�����
		bool IsEnable();
		void Disable();
		void Enable();
		//�¼�
		void Event() override;

	protected:
		float Radius = 5.0f;	// Բ�ǰ뾶
		bool isEnable = true;	// �Ƿ�����
		ClickHandler OnClick;	// �����¼�
	};

}
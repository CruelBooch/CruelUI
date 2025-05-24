#pragma once


#include <d2d1.h>
#include <dwrite.h>
#include <vector>
#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#define CRDT_VCENTER	0x0001
#define CRDT_HCENTER	0x0002
#define CRDT_LEFT		0x0004
#define CRDT_RIGHT		0x0008
#define CRDT_TOP		0x0010
#define CRDT_BOTTOM		0x0020
#define CRDT_CENTER		(CRDT_VCENTER | CRDT_HCENTER)

namespace CruelUI
{
	extern ID2D1Factory* pD2DFactory;
	extern IDWriteFactory* pDWriteFactory;

	typedef D2D1_RECT_F RECTF;
	typedef D2D1_POINT_2F POINTF;
	typedef D2D1::ColorF COLORF;

	inline RECTF RECTtoRECTF(RECT& rect);

	inline RECT  RECTFtoRECT(RECTF& rect);

	inline D2D1::ColorF ARGB(uint8_t  r, uint8_t  g, uint8_t  b, uint8_t  a);



	// ��ʼ��Direct2d����
	HRESULT InitD2D();
	// �ͷ���Դ
	void ReleaseUIResource();

	// ����
	class Font
	{
	public:
		Font();
		Font(std::wstring fontName, float fontSize = 12.0f, bool isBold = false, bool isItalic = false);

		//�����ı��߶�
		void SetTextHeight(float width);
		//�����ı����뷽ʽ
		void SetAlignment(int flags);
		//��ʼ��
		void Init();
		void Release();

		IDWriteTextFormat* pTextFormat = nullptr;	// �ı���ʽ
		IDWriteTextLayout* pTextLayout = nullptr;	// �ı�����
	protected:
		std::wstring fontName;						// ��������
		float fontSize = 12.0f;						// �����С
		bool isBold = false;						// �Ƿ�Ӵ�
		bool isItalic = false;						// �Ƿ�б��
		bool isUnderline = false;					// �Ƿ��»���
		bool isStrikeout = false;					// �Ƿ�ɾ����
		bool isTextHeightSet = false;				// �Ƿ������ı��߶�
	};

	// ����
	class Graphics
	{
	public:
		Graphics(const Graphics&) = delete;
		Graphics();

		void Init(HWND hwnd);
		void Release();

		void BeginDraw() { pRenderTarget->BeginDraw(); }
		void EndDraw() { pRenderTarget->EndDraw(); }

		//��������
		void SetFont(Font font);
		void SetFont(std::wstring fontName, int fontSize = 12.0f, bool isBold = false, bool isItalic = false);
		//��ȡ����
		Font GetFont() { return font; }
		//�����ı����뷽ʽ
		void SetTextAlignment(int flags);

		void Clear(const COLORF& color);

		//�����ı�
		void DrawText_(const std::wstring& text, const RECTF& rect);
		void DrawText_(const std::wstring& text, const POINTF& pos);

		//���������ɫ
		void SetTextColor(const COLORF& color);
		void SetFillColor(const COLORF& color);
		void SetLineColor(const COLORF& color);
		void SetLineWidth(float width);


		//���ƾ���
		void DrawRectangle(const RECTF& rect);
		//����Բ�Ǿ���
		void DrawRoundRect(const RECTF& rect, float radius_x, float radius_y);
		//������Բ
		void DrawEllipse(const RECTF& rect);
		//����Բ
		void DrawCicle(const POINTF& center, float radius);
		//��������
		void DrawLine(const POINTF& start, const POINTF& end);
		//���ƶ����
		void DrawPolygon(const std::vector<POINTF>& points);

		//������
		void SolidRectangle(const RECTF& rect);
		//���Բ�Ǿ���
		void SolidRoundRect(const RECTF& rect, float radius_x, float radius_y);
		//�����Բ
		void SolidEllipse(const RECTF& rect);
		//�������
		void SolidPolygon(const std::vector<POINTF>& points);

		//����ͼƬ
		void DrawImage(ID2D1Bitmap* bitmap, const RECTF& rect);

		//���ô�С
		void Resize(int width, int height);
	protected:
		ID2D1HwndRenderTarget* pRenderTarget = nullptr;		// ��ȾĿ��
		float LineWidth = 1.0f;								// �������
		ID2D1SolidColorBrush* pLineBrush = nullptr;			// ������ˢ
		ID2D1SolidColorBrush* pFillBrush = nullptr;			// ��仭ˢ
		ID2D1SolidColorBrush* pTextBrush = nullptr;			// �ı���ˢ
		Font font;											// ����
	};
}
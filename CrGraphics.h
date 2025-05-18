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



	// 初始化Direct2d环境
	HRESULT InitD2D();
	// 释放资源
	void ReleaseUIResource();

	// 字体
	class Font
	{
	public:
		Font();
		Font(std::wstring fontName, float fontSize = 16, bool isBold = false, bool isItalic = false);

		//设置文本高度
		void SetTextHeight(float width);
		//设置文本对齐方式
		void SetAlignment(int flags);

	protected:
		IDWriteTextFormat* pTextFormat = nullptr;	// 文本格式
		IDWriteTextLayout* pTextLayout = nullptr;	// 文本布局
	};

	// 画布
	class Graphics
	{
	public:
		Graphics(const Graphics&) = delete;

		void Init(HWND hwnd);
		void Release();

		void BeginDraw() { pRenderTarget->BeginDraw(); }
		void EndDraw() { pRenderTarget->BeginDraw(); }

		//设置字体
		void SetFont(Font font);
		void SetFont(std::wstring fontName, int fontSize = 16, bool isBold = false, bool isItalic = false);

		//绘制文本
		void DrawText_(const std::wstring& text, const RECTF& rect);
		void DrawText_(const std::wstring& text, const POINTF& pos);

		//设置填充颜色
		void SetFillColor(const COLORF& color);
		void SetLineColor(const COLORF& color);
		void SetLineWidth(float width);


		//绘制矩形
		void DrawRectangle(const RECTF& rect);
		//绘制圆角矩形
		void DrawRoundRect(const RECTF& rect, float radius_x, float radius_y);
		//绘制椭圆
		void DrawEllipse(const RECTF& rect);
		//绘制圆
		void DrawCicle(const POINTF& center, float radius);
		//绘制线条
		void DrawLine(const POINTF& start, const POINTF& end);
		//绘制多边形
		void DrawPolygon(const std::vector<POINTF>& points);

		//填充矩形
		void SolidRectangle(const RECTF& rect);
		//填充圆角矩形
		void SolidRoundRect(const RECTF& rect, float radius_x, float radius_y);
		//填充椭圆
		void SolidEllipse(const RECTF& rect);
		//填充多边形
		void SolidPolygon(const std::vector<POINTF>& points);

		//绘制图片
		void DrawImage(ID2D1Bitmap* bitmap, const RECTF& rect);

		//设置大小
		void Resize(int width, int height);
	protected:
		ID2D1HwndRenderTarget* pRenderTarget = nullptr;		// 渲染目标
		float LineWidth = 1.0f;								// 线条宽度
		ID2D1SolidColorBrush* pLineBrush = nullptr;			// 线条画刷
		ID2D1SolidColorBrush* pFillBrush = nullptr;			// 填充画刷
	};
}
#include "CrGraphics.h"
#include "CruelUI.h"

namespace CruelUI
{
	ID2D1Factory* pD2DFactory = nullptr;
	IDWriteFactory* pDWriteFactory = nullptr;

	RECTF RECTtoRECTF(RECT& rect)
	{
		RECTF Rect = { rect.left, rect.top, rect.right, rect.bottom };
		return Rect;
	}

	RECT RECTFtoRECT(RECTF& rect)
	{
		RECT Rect = { rect.left, rect.top, rect.right, rect.bottom };
		return Rect;
	}

	D2D1::ColorF ARGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		uint32_t hex = (r << 16) | (g << 8) | b;
		D2D1::ColorF col = D2D1::ColorF(hex, a / 255.0f);
		return col;
	}

	HRESULT InitD2D()
	{
		//创建D2D工厂
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		if (FAILED(hr))
		{
			Log(L"Failed to Create D2D Factory", LWARNING);
		}
		else
		{
			Log(L"D2D Factory Already Created", LINFO);
		}

		//创建DWrite工厂
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory)
		);

		return hr;
	}

	void ReleaseUIResource()
	{
		SafeRelease(&pD2DFactory);
		SafeRelease(&pDWriteFactory);
	}

	Font::Font()
	{
	}

	Font::Font(std::wstring fontName, float fontSize, bool isBold, bool isItalic)
	{
		this->fontName = fontName;
		this->fontSize = fontSize;
		this->isBold = isBold;
		this->isItalic = isItalic;
	}

	void Font::SetTextHeight(float width)
	{
	}

	void Font::SetAlignment(int flags)
	{
		if (flags & CRDT_LEFT)
		{
			pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (flags & CRDT_RIGHT)
		{
			pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		}
		if (flags & CRDT_TOP)
		{
			pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		}
		else if (flags & CRDT_BOTTOM)
		{
			pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		}
		if (flags & CRDT_VCENTER)
		{
			pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
		if (flags & CRDT_HCENTER)
		{
			pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		}
	}

	void Font::Init()
	{
		pDWriteFactory->CreateTextFormat(
			fontName.c_str(),
			NULL,
			isBold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
			isItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"zh-CN",
			&pTextFormat
		);
	}

	void Font::Release()
	{
		SafeRelease(&pTextFormat); 
		SafeRelease(&pTextLayout);
	}

	Graphics::Graphics()
	{
	}

	void Graphics::Init(HWND hwnd)
	{
		HRESULT hr = pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hwnd,
				D2D1::SizeU(800, 600),
				D2D1_PRESENT_OPTIONS_IMMEDIATELY
			),
			&pRenderTarget
		);

		if (FAILED(hr))
		{
			Log(L"Failed to Create D2D Render Target", LERROR);
		}
		else
		{
			Log(L"D2D Render Target Created", LINFO);
		}

		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0x000000, 1.0f),
			&pFillBrush
		);

		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0x000000, 1.0f),
			&pLineBrush
		);

		pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0x000000, 1.0f),
			&pTextBrush
		);

		font.Init();
	}

	void Graphics::Release()
	{
		SafeRelease(&pRenderTarget);
		SafeRelease(&pFillBrush);
		SafeRelease(&pLineBrush);
		SafeRelease(&pTextBrush);
		font.Release();
	}

	void Graphics::SetFont(Font font)
	{
		this->font = font;
	}

	void Graphics::SetFont(std::wstring fontName, int fontSize, bool isBold, bool isItalic)
	{
		this->font = Font(fontName, fontSize, isBold, isItalic);
		this->font.Init();
	}

	void Graphics::SetTextAlignment(int flags)
	{
		font.SetAlignment(flags);
	}

	void Graphics::Clear(const COLORF& color)
	{
		pRenderTarget->Clear(color);
	}

	void Graphics::DrawText_(const std::wstring& text, const RECTF& rect)
	{
		pRenderTarget->DrawText(
			text.c_str(),
			text.length(),
			font.pTextFormat,
			rect,
			pTextBrush
		);
	}

	void Graphics::SetTextColor(const COLORF& color)
	{
		pTextBrush->SetColor(color);
	}

	void Graphics::SetFillColor(const COLORF& color)
	{
		pFillBrush->SetColor(color);
	}

	void Graphics::SetLineColor(const COLORF& color)
	{
		pLineBrush->SetColor(color);
	}

	void Graphics::SetLineWidth(float width)
	{
		LineWidth = width;
	}

	void Graphics::DrawRectangle(const RECTF& rect)
	{
		pRenderTarget->DrawRectangle(rect, pLineBrush, LineWidth);
	}

	void Graphics::DrawRoundRect(const RECTF& rect, float radius_x, float radius_y)
	{
		pRenderTarget->DrawRoundedRectangle(
			D2D1::RoundedRect(rect, radius_x, radius_y),
			pLineBrush,
			LineWidth
		);
	}

	void Graphics::DrawEllipse(const RECTF& rect)
	{
		pRenderTarget->DrawEllipse(
			D2D1::Ellipse(
				D2D1::Point2F((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2),
				(rect.right - rect.left) / 2,
				(rect.bottom - rect.top) / 2
			),
			pLineBrush,
			LineWidth
		);
	}

	void Graphics::DrawCicle(const POINTF& center, float radius)
	{
		pRenderTarget->DrawEllipse(
			D2D1::Ellipse(center, radius, radius),
			pLineBrush,
			LineWidth 
		);
	}

	void Graphics::DrawLine(const POINTF& start, const POINTF& end)
	{
		pRenderTarget->DrawLine(start, end, pLineBrush, LineWidth);
	}

	void Graphics::DrawPolygon(const std::vector<POINTF>& points)
	{
		if (points.size() < 3)
		{
			Log(L"Polygon Points Count Less Than 3", LERROR);
			return;
		}
		ID2D1PathGeometry* pPathGeometry = nullptr;
		pD2DFactory->CreatePathGeometry(&pPathGeometry);
		ID2D1GeometrySink* pSink = nullptr;
		pPathGeometry->Open(&pSink);
		pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
		for (size_t i = 1; i < points.size(); ++i)
		{
			pSink->AddLine(points[i]);
		}
		pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		pSink->Close();
		pRenderTarget->DrawGeometry(pPathGeometry, pLineBrush, LineWidth);
		SafeRelease(&pPathGeometry);
		SafeRelease(&pSink);
	}

	void Graphics::SolidRectangle(const RECTF& rect)
	{
		pRenderTarget->FillRectangle(rect, pFillBrush);
	}

	void Graphics::SolidRoundRect(const RECTF& rect, float radius_x, float radius_y)
	{
		pRenderTarget->FillRoundedRectangle(
			D2D1::RoundedRect(rect, radius_x, radius_y),
			pFillBrush
		);
	}

	void Graphics::SolidEllipse(const RECTF& rect)
	{
		pRenderTarget->FillEllipse(
			D2D1::Ellipse(
				D2D1::Point2F((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2),
				(rect.right - rect.left) / 2,
				(rect.bottom - rect.top) / 2
			),
			pFillBrush
		);
	}

	void Graphics::SolidPolygon(const std::vector<POINTF>& points)
	{

	}

	void Graphics::Resize(int width, int height)
	{
		if (pRenderTarget)
		{
			pRenderTarget->Resize(D2D1::SizeU(width, height));
		}
		else
		{
			Log(L"Render Target is NULL", LERROR);
		}
	}
}

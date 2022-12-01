#include "Editor.h"
#include "Exception.h"

#include <windows.h>

#include <wincodec.h>
#pragma comment(lib, "WindowsCodecs")

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dxguid")

#include <atlbase.h>

Editor::Editor() noexcept
{
}

Editor::~Editor() noexcept
{
	wicFactory->Release();
	factory->Release();
	renderTarget->Release();
	screenshot->Release();
}

void Editor::Initialize(HWND targetWindow)
{
	this->targetWindow = targetWindow;

	dpi = static_cast<float>(GetDpiForWindow(targetWindow));

	HRESULT hr{};
	THROW_IF_FAILED_HR(CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&wicFactory)
	));

	THROW_IF_FAILED_HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory));

	RECT clientRect{};
	GetClientRect(targetWindow, &clientRect);
	D2D1_SIZE_U windowSize{ D2D1::SizeU(clientRect.right, clientRect.bottom) };

	THROW_IF_FAILED_HR(factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_SOFTWARE,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED
			),
			dpi, dpi
		),
		D2D1::HwndRenderTargetProperties(targetWindow, windowSize),
		&renderTarget
	));
}

void Editor::SetScreenshot(HBITMAP screenshot)
{
	HRESULT hr{};

	CComPtr<IWICBitmap> wicBitmap{};
	THROW_IF_FAILED_HR(wicFactory->CreateBitmapFromHBITMAP(
		screenshot,
		NULL,
		WICBitmapAlphaChannelOption::WICBitmapUsePremultipliedAlpha,
		&wicBitmap
	));

	CComPtr<IWICFormatConverter> formatConverter{};
	THROW_IF_FAILED_HR(wicFactory->CreateFormatConverter(&formatConverter));

	THROW_IF_FAILED_HR(formatConverter->Initialize(
		wicBitmap,
		GUID_WICPixelFormat32bppPRGBA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeCustom
	));

	THROW_IF_FAILED_HR(renderTarget->CreateBitmapFromWicBitmap(
		formatConverter,
		D2D1::BitmapProperties(
			D2D1::PixelFormat(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED
			),
			dpi, dpi
		),
		&this->screenshot
	));
}

void Editor::Render() const
{
	renderTarget->BeginDraw();
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black, 1.0f));

	RECT clientRect{};
	THROW_IF_LAST_FAILED(GetClientRect(targetWindow, &clientRect));

	renderTarget->DrawBitmap(
		screenshot,
		D2D1::RectF(
			0.0f,
			0.0f,
			static_cast<FLOAT>(clientRect.right) / GetDpiScale(),
			static_cast<FLOAT>(clientRect.bottom) / GetDpiScale()
		),
		screenshotOpacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);

	HRESULT hr{};
	THROW_IF_FAILED_HR(renderTarget->EndDraw());
}

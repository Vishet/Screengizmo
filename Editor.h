#pragma once

#include <windows.h>

#include <wincodec.h>
#pragma comment(lib, "WindowsCodecs")

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dxguid")

class Editor
{
private:
	float dpi{};
	IWICImagingFactory* wicFactory{};
	ID2D1Factory* factory{};
	ID2D1HwndRenderTarget* renderTarget{};
	ID2D1Bitmap* screenshot{};
	HWND targetWindow{};
	static constexpr float screenshotOpacity{ 0.5f };

public:
	Editor(const Editor&) = delete;
	Editor(const Editor&&) = delete;
	Editor operator=(const Editor&) = delete;
	Editor operator=(const Editor&&) = delete;

	Editor() noexcept;
	~Editor() noexcept;

	void Initialize(HWND targetWindow);
	void SetScreenshot(HBITMAP screenshot);
	void Render() const;

private:
	float GetDpiScale() const noexcept { return dpi / 96.0f; }
};


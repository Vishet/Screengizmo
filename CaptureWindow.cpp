#include "CaptureWindow.h"
#include "Exception.h"

CaptureWindow::CaptureWindow(Keyboard* keyboard) noexcept :
	GenericWindow("", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true, WS_POPUP, keyboard)
{
}

CaptureWindow::~CaptureWindow() noexcept
{
	DeleteObject(backgroundBitmap);
}

HBITMAP CaptureWindow::Capture() const
{
	HDC screenDC{ GetDC(NULL) };
	THROW_IF_FAILED_GDI(screenDC, "Could not get the screen device context");

	HDC memoryDC{ CreateCompatibleDC(screenDC) };
	THROW_IF_FAILED_GDI(memoryDC, "Could not create a device context compatible with the screen");

	int screenWidth{ GetSystemMetrics(SM_CXSCREEN) };
	int screenHeight{ GetSystemMetrics(SM_CYSCREEN) };

	if (backgroundBitmap) 
		DeleteObject(backgroundBitmap);
	HBITMAP backgroundBitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);
	THROW_IF_FAILED_GDI(backgroundBitmap, "Could not create a bitmap compatible with the screen");

	THROW_IF_FAILED_GDI(
		SelectObject(memoryDC, backgroundBitmap),
		"Could not select a bitmap as object of the screen compatible DC"
	);

	THROW_IF_FAILED_GDI(
		BitBlt(memoryDC, 0, 0, screenWidth, screenHeight, screenDC, 0, 0, SRCCOPY),
		"Could not copy the screen DC into a memory DC"
	);

	DeleteDC(memoryDC);
	ReleaseDC(NULL, screenDC);

	return backgroundBitmap;
}

bool CaptureWindow::PeekSaveRequest() noexcept
{
	if (isSaveRequested)
	{
		isSaveRequested = false;
		return true;
	}

	return false;
}

LRESULT CaptureWindow::WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	return 0;
}

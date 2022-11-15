#include "CaptureWindow.h"
#include "Exception.h"

#define VK_S 0x53

CaptureWindow::CaptureWindow() noexcept :
	GenericWindow("", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true, WS_POPUP)
{
}

CaptureWindow::~CaptureWindow() noexcept
{
}

void CaptureWindow::Capture() const
{
	HDC screenDC{ GetDC(NULL) };
	THROW_IF_FAILED_GDI(screenDC, "Could not get the screen device context");

	HDC memoryDC{ CreateCompatibleDC(screenDC) };
	THROW_IF_FAILED_GDI(memoryDC, "Could not create a device context compatible with the screen");

	int screenWidth{ GetSystemMetrics(SM_CXSCREEN) };
	int screenHeight{ GetSystemMetrics(SM_CYSCREEN) };

	HBITMAP bitmapHandle{ CreateCompatibleBitmap(screenDC, screenWidth, screenHeight) };
	THROW_IF_FAILED_GDI(bitmapHandle, "Could not create a bitmap compatible with the screen");

	THROW_IF_FAILED_GDI(
		SelectObject(memoryDC, bitmapHandle),
		"Could not select a bitmap as object of the screen compatible DC"
	);

	THROW_IF_FAILED_GDI(
		BitBlt(memoryDC, 0, 0, screenWidth, screenHeight, screenDC, 0, 0, SRCCOPY),
		"Could not copy the screen DC into a memory DC"
	);

	HDC windowDC{ GetDC(GetWindowHandle()) };
	THROW_IF_FAILED_GDI(
		BitBlt(windowDC, 0, 0, screenWidth, screenHeight, memoryDC, 0, 0, SRCCOPY),
		"Could not copy the memory DC into the window DC"
	);

	DeleteObject(bitmapHandle);
	DeleteDC(memoryDC);
	ReleaseDC(NULL, screenDC);
	ReleaseDC(GetWindowHandle(), windowDC);
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
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			Hide();
			break;
		case VK_S:
			if (GetKeyState(VK_LCONTROL))
			{
				isSaveRequested = true;
			}
			break;
		}
		break;
	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	return 0;
}

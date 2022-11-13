#include "CaptureWindow.h"

CaptureWindow::CaptureWindow() noexcept :
	GenericWindow("", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), WS_POPUP)
{
}

CaptureWindow::~CaptureWindow() noexcept
{
}

void CaptureWindow::Capture() const noexcept
{
	Show();
}

LRESULT CaptureWindow::WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (message)
	{
	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	return 0;
}

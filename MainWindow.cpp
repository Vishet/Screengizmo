#include "MainWindow.h"

MainWindow::MainWindow() noexcept :
	GenericWindow("Screengizmo", 0, 0, WS_OVERLAPPED | WS_SYSMENU)
{
}

MainWindow::~MainWindow()
{
}

LRESULT MainWindow::WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}
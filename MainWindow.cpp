#include "MainWindow.h"
#include "Exception.h"

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
		break;
	case WM_CREATE:
		SetMenus(windowHandle);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case Menus::HIDE:
		{
			Hide();
		}
		}
		break;
	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}

void MainWindow::SetMenus(HWND windowHandle) const
{
	HMENU menuHandle{ CreateMenu() };
	THROW_IF_LAST_FAILED(menuHandle);
	THROW_IF_LAST_FAILED(AppendMenu(menuHandle, MF_STRING, Menus::CAPTURE, "Capture"));
	THROW_IF_LAST_FAILED(AppendMenu(menuHandle, MF_STRING, Menus::HIDE, "Hide"));
	THROW_IF_LAST_FAILED(SetMenu(windowHandle, menuHandle));
}
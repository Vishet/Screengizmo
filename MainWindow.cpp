#include "MainWindow.h"
#include "Exception.h"

MainWindow* MainWindow::mainWindowSingleton{};

MainWindow::MainWindow(Keyboard* keyboard) noexcept :
	GenericWindow("Screengizmo", 0, 0, false, WS_OVERLAPPED | WS_SYSMENU, keyboard)
{
	mainWindowSingleton = this;
}

MainWindow::~MainWindow()
{
}

bool MainWindow::PeekCaptureRequest() noexcept
{
	if (isCaptureRequested)
	{
		isCaptureRequested = false;
		return true;
	}

	return false;
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
		keyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case Menus::CAPTURE:
			isCaptureRequested = true;
			break;
		case Menus::HIDE:
			Hide();
			break;
		}
		break;
	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	return 0;
}

void MainWindow::SetMenus(HWND windowHandle) const
{
	HMENU menuHandle{ CreateMenu() };
	THROW_IF_LAST_FAILED(menuHandle);
	THROW_IF_LAST_FAILED(AppendMenu(menuHandle, MF_STRING, Menus::CAPTURE, "Capture"));
	THROW_IF_LAST_FAILED(AppendMenu(menuHandle, MF_STRING, Menus::HIDE, "Hide"));
	THROW_IF_LAST_FAILED(SetMenu(windowHandle, menuHandle));
}

LRESULT MainWindow::LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:
			KBDLLHOOKSTRUCT* keyboardStruct{ reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam) };

			if (keyboardStruct->vkCode == VK_SNAPSHOT)
			{
				if(mainWindowSingleton)
					mainWindowSingleton->isCaptureRequested = true;
			}
			break;
		}
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}
#include "GenericWindow.h"
#include "Exception.h"

#include <string>

bool GenericWindow::isClassCreated{ false };

GenericWindow::GenericWindow(
	const std::string& title, 
	int width, int height, bool absoluteDimension,
	DWORD style,
	Keyboard* keyboard
) noexcept :
	title{ title },
	width{ width },
	height{ height },
	absoluteDimension{ absoluteDimension },
	style{ style },
	keyboard{ keyboard }
{
}

GenericWindow::~GenericWindow() noexcept
{
	SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
}

void GenericWindow::Show() const noexcept
{
	ShowWindow(windowHandle, SW_SHOW);
}

void GenericWindow::Hide() const noexcept
{
	ShowWindow(windowHandle, SW_HIDE);
}

bool GenericWindow::isVisible() const noexcept
{
	return IsWindowVisible(windowHandle);
}

void GenericWindow::Initialize()
{
	if (!isClassCreated)
	{
		CreateClass();
		isClassCreated = true;
	}

	RECT rect{};
	rect.right = width;
	rect.bottom = height;
	if(!absoluteDimension)
		THROW_IF_LAST_FAILED(AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true));

	windowHandle = CreateWindowEx(
		NULL,
		"vishet",
		title.c_str(),
		style,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL,
		GetModuleHandle(0),
		this
	);

	THROW_IF_LAST_FAILED(windowHandle);
}

void GenericWindow::HandleInput(UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (message)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if(HIWORD(lParam) & ~KF_REPEAT)
			keyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		keyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		keyboard->OnChar(static_cast<char>(wParam));
		break;

	case WM_KILLFOCUS:
		keyboard->Clear();
		break;
	}
}

LRESULT CALLBACK GenericWindow::WindowProcedureCanalizer(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	if (message == WM_NCCREATE)
	{
		const CREATESTRUCT* const cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		GenericWindow* const windowPointer{ static_cast<GenericWindow*>(cs->lpCreateParams) };
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPointer));
		SetWindowLongPtr(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcedureCanalizer));

		return windowPointer->WindowProcedure(windowHandle, message, wParam, lParam);
	}
	else
	{
		GenericWindow* const windowPointer{ reinterpret_cast<GenericWindow* const>(GetWindowLongPtr(windowHandle, GWLP_USERDATA)) };
		if (windowPointer)
		{
			windowPointer->HandleInput(message, wParam, lParam);
			return windowPointer->WindowProcedure(windowHandle, message, wParam, lParam);
		}
		else
			return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}

void GenericWindow::CreateClass() const
{
	WNDCLASSEX wndClass{ 0 };
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = GetModuleHandle(0);
	wndClass.lpfnWndProc = WindowProcedureCanalizer;
	wndClass.lpszClassName = "vishet";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	THROW_IF_LAST_FAILED(RegisterClassEx(&wndClass));
}
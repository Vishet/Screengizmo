#pragma once

#include "GenericWindow.h"

#include <windows.h>

class MainWindow : public GenericWindow
{
private:
	enum Menus
	{
		CAPTURE,
		HIDE
	};

public:
	MainWindow(const MainWindow&) = delete;
	MainWindow(const MainWindow&&) = delete;
	MainWindow operator=(const MainWindow&) = delete;
	MainWindow operator=(const MainWindow&&) = delete;

	MainWindow() noexcept;
	~MainWindow() noexcept;

private:
	LRESULT WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept override;
	void SetMenus(HWND windowHandle) const;
};
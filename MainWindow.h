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
	bool captureRequested{ false };

public:
	MainWindow(const MainWindow&) = delete;
	MainWindow(const MainWindow&&) = delete;
	MainWindow operator=(const MainWindow&) = delete;
	MainWindow operator=(const MainWindow&&) = delete;

	MainWindow() noexcept;
	~MainWindow() noexcept;

	bool IsCaptureRequested() const noexcept { return captureRequested; }

private:
	LRESULT WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept override;
	void SetMenus(HWND windowHandle) const;
};
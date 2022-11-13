#pragma once

#include "GenericWindow.h"

class CaptureWindow : public GenericWindow
{
private:


public:
	CaptureWindow() noexcept;
	~CaptureWindow() noexcept;

	CaptureWindow(const CaptureWindow&) = delete;
	CaptureWindow(const CaptureWindow&&) = delete;
	CaptureWindow operator=(const CaptureWindow&) = delete;
	CaptureWindow operator=(const CaptureWindow&&) = delete;

	void Capture() const noexcept;

private:
	LRESULT WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept override;
};


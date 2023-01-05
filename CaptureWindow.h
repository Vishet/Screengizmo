#pragma once

#include "GenericWindow.h"
#include "Keyboard.h"

class CaptureWindow : public GenericWindow
{
private:
	bool isSaveRequested{ false };
	HBITMAP backgroundBitmap{};

public:
	CaptureWindow(Keyboard* keyboard) noexcept;
	~CaptureWindow() noexcept;

	CaptureWindow(const CaptureWindow&) = delete;
	CaptureWindow(const CaptureWindow&&) = delete;
	CaptureWindow operator=(const CaptureWindow&) = delete;
	CaptureWindow operator=(const CaptureWindow&&) = delete;

	HBITMAP Capture() const;
	bool PeekSaveRequest() noexcept;

private:
	LRESULT WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept override;
};


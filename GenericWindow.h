#pragma once

#include <windows.h>
#include <string>
#include <functional>

class GenericWindow
{
private:
	HWND windowHandle{};
	static bool isClassCreated;
	std::string title;
	int width;
	int height;
	bool absoluteDimension;
	const DWORD style;

public:
	GenericWindow(const GenericWindow&) = delete;
	GenericWindow(const GenericWindow&&) = delete;
	GenericWindow operator=(const GenericWindow&) = delete;
	GenericWindow operator=(const GenericWindow&&) = delete;

	GenericWindow(
		const std::string& title, 
		int width, int height, bool absoluteDimension,
		DWORD style
	) noexcept;
	~GenericWindow() noexcept;

	void Show() const noexcept;
	void Hide() const noexcept;
	bool isVisible() const noexcept;
	HWND GetWindowHandle() const noexcept { return windowHandle; }
	void Initialize();

private:
	virtual LRESULT WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	static LRESULT CALLBACK WindowProcedureCanalizer(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
	void CreateClass() const;
};


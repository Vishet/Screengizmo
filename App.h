#pragma once

#include "MainWindow.h"
#include "CaptureWindow.h"
#include "FileSaver.h"
#include "Editor.h"
#include <optional>

class App
{
private:
	MainWindow mainWindow{};
	CaptureWindow captureWindow{};
	FileSaver fileSaver{};
	ULONG_PTR gdipToken{};
	Editor editor{};

public:
	App();
	~App();

	App(const App&) = delete;
	App(const App&&) = delete;
	App operator=(const App&) = delete;
	App operator=(const App&&) = delete;

	int Run();

private:
	std::optional<int> ProcessMessages() noexcept;
	void Update();
	void Render() const;
};


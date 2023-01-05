#pragma once

#include "MainWindow.h"
#include "CaptureWindow.h"
#include "FileSaver.h"
#include "Editor.h"
#include "Keyboard.h"
#include <optional>

class App
{
private:
	Keyboard keyboard{};
	MainWindow mainWindow{ &keyboard };
	CaptureWindow captureWindow{ &keyboard };
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


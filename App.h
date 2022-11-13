#pragma once

#include "MainWindow.h"
#include "CaptureWindow.h"
#include <optional>

class App
{
private:
	MainWindow mainWindow{};
	CaptureWindow captureWindow{};

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
	void Update() noexcept;
};


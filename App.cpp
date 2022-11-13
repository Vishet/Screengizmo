#include "App.h"

App::App()
{
	mainWindow.Initialize();
	mainWindow.Show();

	captureWindow.Initialize();
}

App::~App()
{
}

int App::Run()
{
	while (true)
	{
		if (const auto returnCode{ ProcessMessages() })
			return *returnCode;

		Update();
	}
}

std::optional<int> App::ProcessMessages() noexcept
{
	MSG message{};

	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
		if (message.message == WM_QUIT)
			return static_cast<int>(message.wParam);

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return {};
}

void App::Update() noexcept
{
	if (mainWindow.IsCaptureRequested())
	{
		captureWindow.Capture();
	}
}

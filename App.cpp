#include "App.h"

App::App()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	Gdiplus::GdiplusStartupInput gdipInput{};
	Gdiplus::GdiplusStartup(&gdipToken, &gdipInput, NULL);

	mainWindow.Initialize();
	mainWindow.Show();

	captureWindow.Initialize();
}

App::~App()
{
	Gdiplus::GdiplusShutdown(gdipToken);
	CoUninitialize();
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

void App::Update()
{
	if (mainWindow.PeekCaptureRequest())
	{
		captureWindow.Show();
		captureWindow.Capture();
	}
	else if (captureWindow.PeekSaveRequest())
	{
		fileSaver.Save(captureWindow.GetWindowHandle());
		captureWindow.Hide();
	}
}

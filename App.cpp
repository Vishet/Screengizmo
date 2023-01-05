#include "App.h"

App::App()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	Gdiplus::GdiplusStartupInput gdipInput{};
	Gdiplus::GdiplusStartup(&gdipToken, &gdipInput, NULL);

	mainWindow.Initialize();
	mainWindow.Show();

	captureWindow.Initialize();

	editor.Initialize(captureWindow.GetWindowHandle());
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
		Render();
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
	while (auto keyEvent{ keyboard.ReadKey() })
	{
		if (keyEvent->IsPressing())
			switch (keyEvent->GetCode())
			{
			case VK_ESCAPE:
				captureWindow.Hide();
				break;
			case VK_S:
				if (keyboard.IsKeyDown(VK_CONTROL))
				{
					fileSaver.Save(captureWindow.GetWindowHandle());
					captureWindow.Hide();
				}
				break;
			};
	}

	if (mainWindow.PeekCaptureRequest())
	{
		captureWindow.Show();
		HBITMAP screenshot{ captureWindow.Capture() };
		editor.SetScreenshot(screenshot);
	}
}

void App::Render() const
{
	if(captureWindow.isVisible())
		editor.Render();
}

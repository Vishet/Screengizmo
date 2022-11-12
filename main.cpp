#include "MainWindow.h"
#include "Exception.h"

#include <windows.h>

int APIENTRY wWinMain(HINSTANCE instanceHandle, HINSTANCE hPrev, PWSTR cmdLine, int cmdshow)
{
	try
	{
		MainWindow window{};
		window.Initialize();
		window.Show();

		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	catch (const Exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error!", MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error!", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details avaliable!", "Undentified error!", MB_OK | MB_ICONERROR);
	}
	
	return 0;
}
#include "WindowManager.h"

int WINAPI WinMain(const HINSTANCE hInstance, const HINSTANCE hPrevInstance, const PSTR  lpCmdLine, const int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	WindowManager* window = new WindowManager;
	const MSG msg = { nullptr };
	if (!window)
	{
		return 0;
	}
	if (FAILED(window->InitWindow(hInstance, nCmdShow)))
	{
		return 0;
	}
	if (FAILED(window->InitDevice()))
	{
		delete window;
		window = nullptr;
		return 0;
	}
	else
	{
		window->Run();
	}
	
	delete window;
	window = nullptr;
	
	return static_cast<int>(msg.wParam);
}
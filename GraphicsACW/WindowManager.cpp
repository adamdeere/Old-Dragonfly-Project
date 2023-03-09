#include "WindowManager.h"
#include <AntTweakBar.h>



WindowManager::WindowManager() : g_hWnd(nullptr), g_hInst(nullptr), objectManager(nullptr), gm(nullptr)
{
}

WindowManager::WindowManager(const WindowManager & source) : g_hWnd(source.g_hWnd), g_hInst(source.g_hInst), objectManager(source.objectManager), gm(source.gm)
{
}

WindowManager & WindowManager::operator=(const WindowManager & source)
{
	if (this == &source)
	{
		return *this;
	}
	g_hWnd = source.g_hWnd; 
	g_hInst = source.g_hInst; 
	objectManager = source.objectManager; 
	gm = source.gm;

	return *this;
	// TODO: insert return statement here
}


WindowManager::~WindowManager()
{
	delete objectManager;
	delete gm;
}
LRESULT CALLBACK WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	
	HDC hdc;
	if (TwEventWin(hWnd, message, wParam, lParam))
		return 0; // Event has been handled by AntTweakBar
	switch (message)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT WindowManager::InitWindow(const HINSTANCE hInstance, const int nCmdShow)
{
	HRESULT hr;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = nullptr;
	hr = RegisterClassEx(&wcex);
	if (FAILED(hr))
		return hr;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 1: Direct3D 11 Basics",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	//if (!g_hWnd)
		//return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);
	return hr;
}

HRESULT WindowManager::InitDevice()
{
	HRESULT hr;
	
	 gm = new GraphicsManager();
	 
	hr = gm->InitilizeDrivers();
	if (FAILED(hr))
		return hr;

	hr = gm->InitilizeFactory(g_hWnd);
	if (FAILED(hr))
		return hr;

	hr = gm->InitilizeTargetView(g_hWnd);
	if (FAILED(hr))
		return hr;

	hr = gm->CreateDepthStencil(g_hWnd);
	if (FAILED(hr))
		return hr;
	
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	TwWindowSize(1500, 2800);
	
	objectManager = new ObjectManager(gm);
	hr = objectManager->LoadModel(g_hWnd);
	if (FAILED(hr))
		return hr;
	
	return hr;
}

void WindowManager::Run()
{
	MSG msg = { nullptr };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			objectManager->UpdateModel();
			objectManager->RenderModel();
			
		}
	}
	TwTerminate();
	delete gm;
	delete objectManager;
	objectManager = nullptr;
	gm = nullptr;
}

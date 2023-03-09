#pragma once
#include "GraphicsManager.h"
#include "ObjectManager.h"
#include "CameraManager.h"
class WindowManager
{
public:
	WindowManager();
	WindowManager(const WindowManager&  source);
	WindowManager& operator= (const WindowManager&  source);
	~WindowManager();
	
	HRESULT InitWindow(const HINSTANCE hInstance, const int nCmdShow);
	HRESULT InitDevice();
	void Run();
private:
	 HWND g_hWnd;
	 HINSTANCE g_hInst;
	 ObjectManager* objectManager;
	 GraphicsManager*  gm;

};


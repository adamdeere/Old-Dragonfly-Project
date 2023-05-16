#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d11_1.h>
#include <d3d11.h>
#include <directxcolors.h>
#include "Resource.h"
#include <d3dcompiler.h>
#include <directxmath.h>
#include <string>
using namespace DirectX;
class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();

	GraphicsManager(const GraphicsManager&) = delete;
	GraphicsManager& operator=(const GraphicsManager&) = delete;

	HRESULT InitilizeDrivers();
	HRESULT InitilizeFactory(const HWND g_hWnd);
	HRESULT InitilizeTargetView(const HWND g_hWnd);
	HRESULT CreateDepthStencil(const HWND g_hWnd);

	ID3D11DeviceContext* getContext()const;
	ID3D11Device* getDevice()const;
	IDXGISwapChain* getSwapChain()const;
	ID3D11DepthStencilView* getDepthStencilView()const;
	ID3D11RenderTargetView* getRenderTargetView()const;
	D3D_DRIVER_TYPE getDriverType()const;

private:
	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pImmediateContext;
	IDXGISwapChain* g_pSwapChain;
	ID3D11RenderTargetView* g_pRenderTargetView;
	ID3D11DepthStencilView* g_pDepthStencilView;
	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	ID3D11Device1* g_pd3dDevice1;
	ID3D11DeviceContext1* g_pImmediateContext1;
	IDXGISwapChain1* g_pSwapChain1;
	ID3D11Texture2D* g_pDepthStencil;
};

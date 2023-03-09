#include "GraphicsManager.h"
#include <string>
#include <vector>


///to do, write a copy constructer and assignment operator so as to safely copy the object over 
/// so i can get hold of the varaibles without changing them
GraphicsManager::GraphicsManager() :g_pd3dDevice(nullptr), g_pImmediateContext(nullptr), g_pSwapChain(nullptr), g_pRenderTargetView(nullptr), g_pDepthStencilView(nullptr), g_driverType(D3D_DRIVER_TYPE_NULL),
g_featureLevel(D3D_FEATURE_LEVEL_11_0), g_pd3dDevice1(nullptr), g_pImmediateContext1(nullptr), g_pSwapChain1(nullptr), g_pDepthStencil(nullptr)
{
}


GraphicsManager::~GraphicsManager()
{
	try
	{
		if (g_pd3dDevice)g_pd3dDevice->Release();
		if (g_pImmediateContext)g_pImmediateContext->Release();
		if (g_pSwapChain)g_pSwapChain->Release();
		if (g_pRenderTargetView)g_pRenderTargetView->Release();
		if (g_pDepthStencilView)g_pDepthStencilView->Release();
		if (g_pDepthStencil)g_pDepthStencil->Release();
	}
	catch (const std::exception&)
	{
			
	}
	
}

HRESULT GraphicsManager::InitilizeDrivers()
{
	HRESULT hr;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	std::vector<D3D_DRIVER_TYPE> driverTypes;
	driverTypes.push_back(D3D_DRIVER_TYPE_HARDWARE);
	driverTypes.push_back(D3D_DRIVER_TYPE_WARP);
	driverTypes.push_back(D3D_DRIVER_TYPE_REFERENCE);

	const UINT numDriverTypes = driverTypes.size();

	std::vector<D3D_FEATURE_LEVEL> featureLevels;
	featureLevels.push_back(D3D_FEATURE_LEVEL_11_1);
	featureLevels.push_back(D3D_FEATURE_LEVEL_11_0);
	featureLevels.push_back(D3D_FEATURE_LEVEL_10_1);
	featureLevels.push_back(D3D_FEATURE_LEVEL_10_0);
	
	const UINT numFeatureLevels = featureLevels.size();
	//here builds 
	//problem starts here fixed problem by adding the libaries into the linker input directory
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[driverTypeIndex], numFeatureLevels,
			D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (FAILED(hr))
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[driverTypeIndex], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	return hr;
}

HRESULT GraphicsManager::InitilizeFactory(const HWND g_hWnd)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	const UINT width = rc.right - rc.left;
	const UINT height = rc.bottom - rc.top;
	HRESULT hr;// = S_OK;
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			//reinterpret_cast<void**>(g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(g_pImmediateContext1)));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, g_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
		}
	}
	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = g_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
	}
	if (FAILED(hr))
		return hr;
	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	if (dxgiFactory2)dxgiFactory2->Release();
	if (g_pd3dDevice1)g_pd3dDevice1->Release();
	if (g_pImmediateContext1)g_pImmediateContext1->Release();
	if (g_pSwapChain1)g_pSwapChain1->Release();

	return hr;
}

HRESULT GraphicsManager::InitilizeTargetView(const HWND g_hWnd)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	const UINT width = rc.right - rc.left;
	const UINT height = rc.bottom - rc.top;
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	return hr;
}

HRESULT GraphicsManager::CreateDepthStencil(const HWND g_hWnd)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	const UINT width = rc.right - rc.left;
	const UINT height = rc.bottom - rc.top;
	HRESULT hr;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	return hr;
}

ID3D11DeviceContext * GraphicsManager::getContext()const
{
	return g_pImmediateContext;
}

ID3D11Device * GraphicsManager::getDevice()const
{
	return g_pd3dDevice;
}

IDXGISwapChain * GraphicsManager::getSwapChain()const
{
	return g_pSwapChain;
}

ID3D11DepthStencilView * GraphicsManager::getDepthStencilView()const
{
	return g_pDepthStencilView;
}

ID3D11RenderTargetView * GraphicsManager::getRenderTargetView()const
{
	return g_pRenderTargetView;
}

D3D_DRIVER_TYPE GraphicsManager::getDriverType()const
{
	return g_driverType;
}

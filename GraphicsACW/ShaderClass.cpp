#include "ShaderClass.h"

ShaderClass::ShaderClass(const std::string & eFileName, const std::string & vertEntryPoint, const std::string & pixelEntryPoint, const std::string & tag) : effectFileString(eFileName), effectVertexString(vertEntryPoint),
effectPixelString(pixelEntryPoint), shaderTag(tag), g_pVertexShader(nullptr), g_pPixelShader(nullptr),g_pVertexLayout(nullptr)
{
}
ShaderClass::ShaderClass(const ShaderClass & source) :
	effectFileString(source.effectFileString), effectVertexString(source.effectVertexString), effectPixelString(source.effectPixelString), shaderTag(source.shaderTag), 
	g_pVertexShader(source.g_pVertexShader), g_pPixelShader(source.g_pPixelShader),
	g_pVertexLayout(source.g_pVertexLayout)
{
}


ShaderClass & ShaderClass::operator=(const ShaderClass & source)
{
	if (this == &source)
	{
		return *this;
	}
	     effectFileString == source.effectFileString;
		effectVertexString == source.effectVertexString;
		effectPixelString == source.effectPixelString;
		shaderTag == source.shaderTag;
		g_pVertexShader = source.g_pVertexShader;
		g_pPixelShader = source.g_pPixelShader;
		g_pVertexLayout = source.g_pVertexLayout;
	return *this;
	// TODO: insert return statement here
}

ShaderClass::~ShaderClass()
{
	try
	{
		if (g_pVertexShader) g_pVertexShader->Release();
		if (g_pPixelShader) g_pPixelShader->Release();
		if (g_pVertexLayout) g_pVertexLayout->Release();
	}
	catch (const std::exception&)
	{
			
	}
}

HRESULT ShaderClass::LoadShader(ID3D11Device* const g_pd3dDevice, ID3D11DeviceContext* const g_pImmediateContext) 
{
	const std::wstring widestr = std::wstring(effectFileString.begin(), effectFileString.end());
	const WCHAR* const effectFileName = widestr.c_str();
	const LPCSTR  effectVertexEntry = effectVertexString.c_str();
	const LPCSTR  effectPixelEntry = effectPixelString.c_str();
	HRESULT hr;
	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(effectFileName, effectVertexEntry, "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}
	
	//these tags are in place to set up input layouts according to the shader that the model itself will need
	if (shaderTag == "enviromentShader" || shaderTag == "refractedEnviromentShader")
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &g_pVertexLayout);
		pVSBlob->Release();
		if (FAILED(hr))
			return hr;
	}
	else if (shaderTag == "particleShader")
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &g_pVertexLayout);
		pVSBlob->Release();
		if (FAILED(hr))
			return hr;
	}
	
	else if (shaderTag == "objectShader" )
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &g_pVertexLayout);
		pVSBlob->Release();
		if (FAILED(hr))
			return hr;
	}
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(effectFileName, effectPixelEntry, "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;
}

ID3D11VertexShader* ShaderClass::GetVertexShader()const
{
	return g_pVertexShader;
}

ID3D11PixelShader* ShaderClass::GetPixelShader()const
{
	return g_pPixelShader;
}

ID3D11InputLayout* ShaderClass::GetInputLayout()
{
	return g_pVertexLayout;
}

 const std::string& ShaderClass::GetShaderTag()const
{
	return shaderTag;
}

HRESULT ShaderClass::CompileShaderFromFile(const WCHAR* const szFileName, LPCSTR const szEntryPoint, LPCSTR const szShaderModel, ID3DBlob** const ppBlobOut)const
{
	HRESULT hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return hr;
}

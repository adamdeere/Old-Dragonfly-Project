#include "EnviromentObject.h"
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMVECTOR mEyeposition;
};


EnviromentObject::EnviromentObject(): g_pVertexBuffer(nullptr), g_pIndexBuffer(nullptr), g_pConstantBuffer(nullptr), sky_TextureRV(nullptr), sky_Sampler(nullptr),
DepthStencilStateEnviroment(nullptr), m_rasterStateEnviroment(nullptr)
{
}

EnviromentObject::EnviromentObject(const std::string &tag) : g_pVertexBuffer(nullptr), g_pIndexBuffer(nullptr), g_pConstantBuffer(nullptr), sky_TextureRV(nullptr), sky_Sampler(nullptr),
DepthStencilStateEnviroment(nullptr), m_rasterStateEnviroment(nullptr), objectTag(tag)
{
}

//EnviromentObject::EnviromentObject(const EnviromentObject & source) : g_pVertexBuffer(source.g_pVertexBuffer), g_pIndexBuffer(source.g_pIndexBuffer), g_pConstantBuffer(source.g_pConstantBuffer),
//sky_TextureRV(source.sky_TextureRV), sky_Sampler(source.sky_Sampler), DepthStencilStateEnviroment(source.DepthStencilStateEnviroment), m_rasterStateEnviroment(source.m_rasterStateEnviroment), 
//objectTag(source.objectTag)
//{
//}
//
//EnviromentObject & EnviromentObject::operator=(const EnviromentObject & source)
//{
//	if (this == &source)
//	{
//		return *this;
//	}
//	g_World = source.g_World;
//	rasterDescSky = source.rasterDescSky;
//	g_pVertexBuffer = source.g_pVertexBuffer; 
//	g_pIndexBuffer = source.g_pIndexBuffer;
//		g_pConstantBuffer = source.g_pConstantBuffer;
//		sky_TextureRV = source.sky_TextureRV;
//		sky_Sampler = source.sky_Sampler;
//		DepthStencilStateEnviroment = source.DepthStencilStateEnviroment;
//		m_rasterStateEnviroment = source.m_rasterStateEnviroment;
//		objectTag = source.objectTag;
//		DepthStencilStateEnviroment = source.DepthStencilStateEnviroment;
//		
//
//		return *this;
//	// TODO: insert return statement here
//}


EnviromentObject::~EnviromentObject()
{
	try
	{
		if (g_pVertexBuffer)g_pVertexBuffer->Release();
		if (g_pIndexBuffer) g_pIndexBuffer->Release();;
		if (g_pConstantBuffer)g_pConstantBuffer->Release();
		if (sky_TextureRV) sky_TextureRV->Release();
		if (sky_Sampler) sky_Sampler->Release();
		if (DepthStencilStateEnviroment) DepthStencilStateEnviroment->Release();
		if (m_rasterStateEnviroment) m_rasterStateEnviroment->Release();
	}
	catch (const std::exception&)
	{
			
	}
}

HRESULT EnviromentObject::LoadModel(ID3D11Device* const g_pd3dDevice, ID3D11DeviceContext* const g_pImmediateContext)
{
	HRESULT hr;
#pragma region load in a cube.
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),     XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),      XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),     XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),   XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),    XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),     XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),    XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),    XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),   XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),    XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),     XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f),     XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),    XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),     XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),      XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),   XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),    XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),     XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),    XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),    XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),     XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),      XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),     XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Create index buffer
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;
	
#pragma endregion

#pragma region creates constant buffer
	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;


#pragma endregion

#pragma region creates texture

	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Skymap.dds", nullptr, &sky_TextureRV);
	if (FAILED(hr))
		return hr;
#pragma endregion


#pragma region sets up the sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &sky_Sampler);

#pragma endregion

#pragma region set up depth stecil
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xff;
	dsDesc.StencilWriteMask = 0xff;

	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = g_pd3dDevice->CreateDepthStencilState(&dsDesc, &DepthStencilStateEnviroment);
	if (FAILED(hr))
		return hr;
#pragma endregion

#pragma region set up rasterizer state

	rasterDescSky.CullMode = D3D11_CULL_NONE;
	rasterDescSky.FillMode = D3D11_FILL_SOLID;
	rasterDescSky.ScissorEnable = false;
	rasterDescSky.DepthBias = 0;
	rasterDescSky.DepthBiasClamp = 0.0f;
	rasterDescSky.DepthClipEnable = true;
	rasterDescSky.MultisampleEnable = false;
	rasterDescSky.SlopeScaledDepthBias = 0.0f;

	hr = g_pd3dDevice->CreateRasterizerState(&rasterDescSky, &m_rasterStateEnviroment);
	if (FAILED(hr))
		return hr;
#pragma endregion

	
	g_World = XMMatrixIdentity();
}

void EnviromentObject::RenderModel(ID3D11DeviceContext * const g_pImmediateContext, XMMATRIX const & g_View, XMMATRIX const & g_Projection, XMVECTOR const &eyePos, LightManager* const lightManager)
{
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.viewMatrix = XMMatrixTranspose(g_View);
	cb.projectionMatrix = XMMatrixTranspose(g_Projection);
	cb.mEyeposition = eyePos;
	UINT const stride = sizeof(SimpleVertex);
	UINT const offset = 0;

	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->OMSetDepthStencilState(DepthStencilStateEnviroment, 1);
	g_pImmediateContext->RSSetState(m_rasterStateEnviroment);
	
	g_pImmediateContext->PSSetShaderResources(0, 1, &sky_TextureRV);
	g_pImmediateContext->PSSetSamplers(0, 1, &sky_Sampler);
	
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(36, 0, 0);
}

const std::string & EnviromentObject::GetObjectTag()
{
	return objectTag;
}



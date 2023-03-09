#include "ParticleObject.h"



struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	float time;
};

ParticleObject::ParticleObject() : g_pVertexBuffer(nullptr), g_pConstantBuffer(nullptr), DepthStencilStateParticle(nullptr), m_rasterStateParticle(nullptr),
m_blendStateParticle(nullptr), m_NoBlendStateParticle(nullptr), particleTexture(nullptr), particleSampler(nullptr), numberOfParticles(0), t(0)
{
}


ParticleObject::ParticleObject(int const numOfPart) : g_pVertexBuffer(nullptr), g_pConstantBuffer(nullptr), DepthStencilStateParticle(nullptr), m_rasterStateParticle(nullptr),
m_blendStateParticle(nullptr), m_NoBlendStateParticle(nullptr), particleTexture(nullptr), particleSampler(nullptr), numberOfParticles(numOfPart), t(0)
{
}

ParticleObject::ParticleObject(const ParticleObject & source) : g_pVertexBuffer(source.g_pVertexBuffer), g_pConstantBuffer(source.g_pConstantBuffer), DepthStencilStateParticle(source.DepthStencilStateParticle), 
m_rasterStateParticle(source.m_rasterStateParticle),m_blendStateParticle(source.m_blendStateParticle), m_NoBlendStateParticle(source.m_NoBlendStateParticle), particleTexture(source.particleTexture), 
particleSampler(source.particleSampler), numberOfParticles(source.numberOfParticles), t(source.t)
{
}

ParticleObject & ParticleObject::operator=(const ParticleObject & source)
{
	if (this == &source)
	{
		return *this;
	}
	   blendState = source.blendState;
	   g_World = source.g_World;
    	g_pVertexBuffer = source.g_pVertexBuffer;
		g_pConstantBuffer = source.g_pConstantBuffer;
		DepthStencilStateParticle = source.DepthStencilStateParticle;
		m_rasterStateParticle = source.m_rasterStateParticle;
		m_blendStateParticle = source.m_blendStateParticle;
		m_NoBlendStateParticle = source.m_NoBlendStateParticle;
		particleTexture = source.particleTexture;
		particleSampler = source.particleSampler;
		numberOfParticles = source.numberOfParticles;
		t = source.t;
		return *this;
	// TODO: insert return statement here
}

ParticleObject::~ParticleObject()
{
	
}

HRESULT ParticleObject::LoadParticle(ID3D11Device* const g_pd3dDevice)
{
	HRESULT hr;

#pragma region loads in a texture to use
	//todo load in a file and set a sample state
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Tiny\\sandSeemLess.dds", nullptr, &particleTexture);
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

	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &particleSampler);

#pragma endregion

#pragma region creates vertices and vertex buffer
	ParticleVertex* const pVertices = new ParticleVertex[6 * numberOfParticles];
	//every z componet has a seprate vertex to allow the particle to move in diffrent directions
	//c style cast is used becasue the c++ cast breaks the code. I dont know why
	for (int i = 0; i < numberOfParticles; i++)
	{
		const float particleZ = (float)i / numberOfParticles;
    	pVertices[i * 6] =     { XMFLOAT3(-1.0f, 1.0f, particleZ), XMFLOAT2(0.0f, 0.0f) };
	    pVertices[i * 6 + 1] = { XMFLOAT3(1.0f, -1.0f, particleZ), XMFLOAT2(1.0f, 1.0f) };
		pVertices[i * 6 + 2] = { XMFLOAT3(-1.0f, -1.0f,particleZ), XMFLOAT2(0.0f, 1.0f) };
			//second tri
		pVertices[i * 6 + 3] = { XMFLOAT3(-1.0f, 1.0f, particleZ), XMFLOAT2(0.0f, 0.0f) };
		pVertices[i * 6 + 4] = { XMFLOAT3(1.0f,  1.0f, particleZ), XMFLOAT2(1.0f, 0.0f) };
		pVertices[i * 6 + 5] = { XMFLOAT3(1.0f, -1.0f, particleZ), XMFLOAT2(1.0f, 1.0f) };
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ParticleVertex) * (6 * numberOfParticles);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = pVertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
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

	hr = g_pd3dDevice->CreateDepthStencilState(&dsDesc, &DepthStencilStateParticle);
	if (FAILED(hr))
		return hr;
#pragma endregion
	
#pragma region creates a blend state

	//this creates a blend state to enable the blend and one to disable the blend,
	//essetinally clipping the blend between the two
	ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));
	blendState.RenderTarget[0].BlendEnable = TRUE;
	blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = g_pd3dDevice->CreateBlendState(&blendState, &m_blendStateParticle);

	blendState.RenderTarget[0].BlendEnable = FALSE;
	hr = g_pd3dDevice->CreateBlendState(&blendState, &m_NoBlendStateParticle);
	if (FAILED(hr))
		return hr;
#pragma endregion

#pragma region set up rasterizer state
	D3D11_RASTERIZER_DESC rasterDescSky;

	rasterDescSky.CullMode = D3D11_CULL_NONE;
	rasterDescSky.FillMode = D3D11_FILL_SOLID;
	rasterDescSky.ScissorEnable = false;
	rasterDescSky.DepthBias = 0;
	rasterDescSky.DepthBiasClamp = 0.0f;
	rasterDescSky.DepthClipEnable = true;
	rasterDescSky.MultisampleEnable = false;
	rasterDescSky.SlopeScaledDepthBias = 0.0f;

	hr = g_pd3dDevice->CreateRasterizerState(&rasterDescSky, &m_rasterStateParticle);
	if (FAILED(hr))
		return hr;
#pragma endregion

	g_World = XMMatrixIdentity();
	return hr;
}

void ParticleObject::RenderParticle(ID3D11DeviceContext* const g_pImmediateContext, XMMATRIX const &g_View, XMMATRIX const &g_Projection, ID3D11InputLayout* const g_pVertexLayout)
{
	ConstantBuffer cb;
	// t = 0.0f;
	XMMATRIX const scale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	const XMMATRIX mTranslate = XMMatrixTranslation(-1.0f, -1.0f, -1.0f);
	g_World = scale;
	g_World *= mTranslate;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection); 
	static ULONGLONG timeStart = 0;
	ULONGLONG const timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;

	t = (timeCur - timeStart) / 1000.0f;
	cb.time = t;
	UINT const stride = sizeof(ParticleVertex);
	UINT const offset = 0;
	
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	g_pImmediateContext->OMSetBlendState(m_blendStateParticle, nullptr, 1);
	g_pImmediateContext->OMSetDepthStencilState(DepthStencilStateParticle, 1);
	g_pImmediateContext->RSSetState(m_rasterStateParticle);
	g_pImmediateContext->PSSetShaderResources(0, 1, &particleTexture);
	g_pImmediateContext->PSSetSamplers(0, 1, &particleSampler);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->Draw(6 * numberOfParticles, 0);

	g_pImmediateContext->OMSetBlendState(m_NoBlendStateParticle, nullptr, 1);

}

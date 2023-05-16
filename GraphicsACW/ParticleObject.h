#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <string>
#include <directxmath.h>
#include "DDSTextureLoader.h"
#include <vector>
using namespace DirectX;
struct ParticleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};
class ParticleObject
{
public:
	explicit ParticleObject();
	explicit ParticleObject(int const numOfPart);
	ParticleObject(const ParticleObject& source);
	ParticleObject& operator= (const ParticleObject& source);
	~ParticleObject();
	HRESULT LoadParticle(ID3D11Device* const g_pd3dDevice);
	void RenderParticle(ID3D11DeviceContext* const g_pImmediateContext, XMMATRIX const& g_View, XMMATRIX const& g_Projection, ID3D11InputLayout* const g_pVertexLayout);

private:

	D3D11_BLEND_DESC blendState;
	ID3D11Buffer* g_pVertexBuffer;
	ID3D11Buffer* g_pConstantBuffer;
	ID3D11DepthStencilState* DepthStencilStateParticle;
	ID3D11RasterizerState* m_rasterStateParticle;
	ID3D11BlendState* m_blendStateParticle;
	ID3D11BlendState* m_NoBlendStateParticle;
	ID3D11ShaderResourceView* particleTexture;
	ID3D11SamplerState* particleSampler;
	XMMATRIX            g_World;
	int numberOfParticles;
	float t;
};

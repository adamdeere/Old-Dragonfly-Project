#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <string>
#include <directxmath.h>
#include "DDSTextureLoader.h"
#include "ShaderClass.h"
#include "ProjectObject.h"
#include "LightManager.h"
using namespace DirectX;
class EnviromentObject : public ProjectObject
{
public:
	explicit EnviromentObject();
	explicit EnviromentObject(const std::string  &tag);
	EnviromentObject(const EnviromentObject&  source);
	EnviromentObject& operator= (const EnviromentObject&  source);
	 ~EnviromentObject() override;

	 HRESULT LoadModel(ID3D11Device* const pd3dDevice, ID3D11DeviceContext* const pd3dImmediateContext) override;
	 void RenderModel(ID3D11DeviceContext* const pd3dImmediateContext, XMMATRIX const &g_View, XMMATRIX const &g_Projection, XMVECTOR const &eyePos, LightManager* const  lightManager) override;
	 const std::string& GetObjectTag() override;
private:

	D3D11_RASTERIZER_DESC rasterDescSky;

	XMMATRIX            g_World;

	ID3D11Buffer* g_pVertexBuffer;
	ID3D11Buffer* g_pIndexBuffer;
	ID3D11Buffer* g_pConstantBuffer;

	ID3D11ShaderResourceView* sky_TextureRV;
	ID3D11SamplerState* sky_Sampler;
	ID3D11DepthStencilState* DepthStencilStateEnviroment;
	ID3D11RasterizerState* m_rasterStateEnviroment;

	std::string objectTag;
	
};


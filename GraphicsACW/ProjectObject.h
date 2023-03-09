#pragma once
#include <windows.h>
#include "DXUT.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include <string>
#include <AntTweakBar.h>
#include "DDSTextureLoader.h"
#include "LightManager.h"
#include "ShaderClass.h"
using namespace DirectX;

class ProjectObject
{
public:
	ProjectObject();
	ProjectObject(std::string const &normalMap, std::string const &shaderName, std::string const &objectName, std::string const &meshFilr);
	ProjectObject(const ProjectObject&  source);
	ProjectObject& operator= (const ProjectObject&  source);
	virtual ~ProjectObject();

	//ProjectObject(const ProjectObject&) = delete;
	//ProjectObject& operator= (const ProjectObject&) = delete;


	virtual HRESULT LoadModel(ID3D11Device* const pd3dDevice, ID3D11DeviceContext* const pd3dImmediateContext);
	virtual void UpdateModel(int const &renderNo, ID3D11Device* const pd3dDevice);
	virtual void RenderModel(ID3D11DeviceContext* const pd3dImmediateContext, XMMATRIX const &g_View, XMMATRIX const &g_Projection, XMVECTOR const &eyePos, LightManager* const  lightManager);
	virtual void ResetModel();

   virtual const std::string& GetObjectTag();
 
	virtual void SetReflectionTexture(ID3D11ShaderResourceView* const &value);
	virtual void  GetCam(std::string const &value);

	virtual void SetCurrentShader(ShaderClass* const currentShader);
	virtual bool GetTakenOffBool();
#pragma region call backs to all of the ant tweak bar parameters
	//call back to set the translation of an object
	
	
#pragma endregion

	

	
private:
	CDXUTSDKMesh  g_Mesh;
	ID3D11ShaderResourceView* normalTexture;
	ID3D11SamplerState* g_pSamplerLinear;
	ID3D11Buffer* g_pConstantBuffer;
	XMMATRIX m_Model;
	D3D11_RASTERIZER_DESC rasterDesc;
	std::string normalMapFilePath;
	ID3D11DepthStencilState* DepthStencilStateObject;
	std::string shaderTag;
	ID3D11RasterizerState* 	m_RasterState;
	int renderNumber;
	ID3D11ShaderResourceView* reflectionTexture;
	std::string camTag;
	std::string objectTag;
	std::string modelFile;
	ShaderClass* shader;
};


#pragma once
#include "ProjectObject.h"
#include "ShaderClass.h"
#include "ParticleObject.h"
#include "LightManager.h"
#include "DDSTextureLoader.h"
#include <string>
#include "CollisionManager.h"
#include "Timer.h"
class DragonFlyObject : public ProjectObject
{
public:
	DragonFlyObject();
	DragonFlyObject::DragonFlyObject(std::string const &ShaderName, std::string const &name);
	DragonFlyObject(const DragonFlyObject&  source);
	DragonFlyObject& operator= (const DragonFlyObject&  source);
	~DragonFlyObject() override;
	HRESULT LoadModel(ID3D11Device* const pd3dDevice, ID3D11DeviceContext* const pd3dImmediateContext) override;
	void UpdateModel(int const &renderNo, ID3D11Device* const pd3dDevice) override;
    void RenderModel(ID3D11DeviceContext* const pd3dImmediateContext, XMMATRIX const &g_View, XMMATRIX const &g_Projection, XMVECTOR const &eyePos, LightManager* const lightManager) override;

	const std::string& GetObjectTag() override;

	const XMFLOAT3& GetTranslation()const ;
	void SetTranslation(XMFLOAT3 const &transValue);

	const XMFLOAT3& GetScale()const ;
	void SetScale(XMFLOAT3 const &scaleValue);

	const XMFLOAT3& GetRotation() const;
	void SetRoation(XMFLOAT3 const &rotValue) ;
	void ResetModel() override;

	void  GetCam(std::string const &value) override;
    void SetReflectionTexture(ID3D11ShaderResourceView* const &value) override;
	void SetCurrentShader(ShaderClass* const shader) override;
	bool GetTakenOffBool() override;

	static void TW_CALL GetTranslationCallback(void *value, void *clientData)
	{
		*static_cast<XMFLOAT3*>(value) = static_cast<DragonFlyObject *>(clientData)->GetTranslation();
	}

	static void TW_CALL SetRotationCallback(void const *value, void *clientData)
	{
		static_cast<DragonFlyObject*>(clientData)->SetRoation(*static_cast<const XMFLOAT3 *>(value));
	}
	static void TW_CALL GetRotationCallback(void *value, void *clientData)
	{
		*static_cast<XMFLOAT3 *>(value) = static_cast<const DragonFlyObject*>(clientData)->GetRotation();
	}

private :
	XMFLOAT3 modelRotate;
	XMFLOAT3 modelTranslate;
	XMFLOAT3 modelScale;
	


	CDXUTSDKMesh  bodyMesh;
	CDXUTSDKMesh  wingMesh;
	CDXUTSDKMesh  lEyeMesh;
	CDXUTSDKMesh  rEyeMesh;

	std::string camTag;
	std::string shaderTag;
	std::string animationShaderTag;
	std::string objectName;
	bool flying;

	ID3D11ShaderResourceView* normalTextureBody;
	ID3D11ShaderResourceView* normalTextureWing;
	ID3D11ShaderResourceView* normalTextureEye;
    ID3D11ShaderResourceView* reflectedEyeTextureFromObjectManager;

	ShaderClass* eyeShader;
	ShaderClass* wingShader;
	ShaderClass* particleShader;
	ShaderClass* currentShader;

	
	ID3D11ShaderResourceView* normalTexture;
	ID3D11SamplerState* g_pSamplerLinear;
	ID3D11Buffer* g_pConstantBuffer;
	XMMATRIX m_Model;
	D3D11_RASTERIZER_DESC rasterDesc;
	
	ID3D11DepthStencilState* DepthStencilStateObject;
	
	CollisionManager* collisionMnager;

	ID3D11RasterizerState* 	m_RasterState;
	//std::vector<CDXUTSDKMesh> dFlyMeshList;
	std::vector<CollisionManager*> collsionList;
	int renderNumber;
	void RenderBodyMethod(CDXUTSDKMesh const &mesh, ID3D11DeviceContext* const pd3dImmediateContext, ID3D11ShaderResourceView* const reflectedEyeTexture)const;
	void RenderEyeMethod(CDXUTSDKMesh const &mesh, ID3D11DeviceContext* const pd3dImmediateContext, ID3D11ShaderResourceView* const reflectedEyeTexture)const;
	XMFLOAT3 velocity;
	bool disturbed;
	float t;
	Timer ft;
	bool acsedning;
	bool decending;
	bool hover;
};


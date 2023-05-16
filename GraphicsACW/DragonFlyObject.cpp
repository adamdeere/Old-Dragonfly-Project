#include "DragonFlyObject.h"
#include <iostream>
using namespace std;
struct ConstantBuffer
{
	XMMATRIX modelMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	float time;

	XMFLOAT3 lightPos;

	XMVECTOR mEyeposition;

	XMFLOAT4 vLightDir[4];
	XMFLOAT4 vLightColor[4];

	XMMATRIX rotationMatrix;
};

DragonFlyObject::DragonFlyObject() : bodyMesh(), flying(false), normalTextureBody(nullptr), normalTextureWing(nullptr), normalTextureEye(nullptr), reflectedEyeTextureFromObjectManager(nullptr),
eyeShader(nullptr), wingShader(nullptr), particleShader(nullptr), currentShader(nullptr), normalTexture(nullptr), g_pSamplerLinear(nullptr), g_pConstantBuffer(nullptr), DepthStencilStateObject(nullptr), collisionMnager(nullptr),
m_RasterState(nullptr), renderNumber(0), disturbed(false), t(0), acsedning(false), decending(false), hover(false)
{
}

DragonFlyObject::DragonFlyObject(std::string const& ShaderName, std::string const& name) :
	bodyMesh(), animationShaderTag(ShaderName), objectName(name), flying(false), normalTextureBody(nullptr), normalTextureWing(nullptr), normalTextureEye(nullptr), reflectedEyeTextureFromObjectManager(nullptr),
	eyeShader(nullptr), wingShader(nullptr), particleShader(nullptr), currentShader(nullptr), normalTexture(nullptr), g_pSamplerLinear(nullptr), g_pConstantBuffer(nullptr), DepthStencilStateObject(nullptr), collisionMnager(nullptr),
	m_RasterState(nullptr), renderNumber(0), disturbed(false), t(0), acsedning(false), decending(false), hover(false)
{
}
//DragonFlyObject::DragonFlyObject(const DragonFlyObject & source) : g_Mesh(source.g_Mesh), bodyMesh(source.bodyMesh), animationShaderTag(source.animationShaderTag), objectName(source.objectName),
//flying(source.flying), normalTextureBody(source.normalTextureBody), normalTextureWing(source.normalTextureWing), normalTextureEye(source.normalTextureEye), reflectedEyeTextureFromObjectManager(source.reflectedEyeTextureFromObjectManager),
//eyeShader(source.eyeShader), wingShader(source.wingShader), particleShader(source.particleShader), currentShader(source.currentShader), normalTexture(source.normalTexture), g_pSamplerLinear(source.g_pSamplerLinear),
//g_pConstantBuffer(source.g_pConstantBuffer), DepthStencilStateObject(source.DepthStencilStateObject), collisionMnager(source.collisionMnager),
//m_RasterState(source.m_RasterState), renderNumber(source.renderNumber), disturbed(source.disturbed), t(source.t)
//{
//}
//DragonFlyObject & DragonFlyObject::operator=(const DragonFlyObject & source)
//{
//	if (this == &source)
//	{
//		return *this;
//	}
//	DepthStencilStateObject = source.DepthStencilStateObject;
//	animationShaderTag = source.animationShaderTag;
//	bodyMesh = source.bodyMesh;
//	camTag = source.camTag;
//	collisionMnager = source.collisionMnager;
//	collsionList = source.collsionList;
//	currentShader = source.currentShader;
//	dFlyMeshList = source.dFlyMeshList;
//	disturbed = source.disturbed;
//	eyeShader = source.eyeShader;
//	flying = source.flying;
//	g_Mesh = source.g_Mesh;
//	g_pConstantBuffer = source.g_pConstantBuffer;
//	g_pSamplerLinear = source.g_pSamplerLinear;
//	lEyeMesh = source.lEyeMesh;
//	m_Model = source.m_Model;
//	m_RasterState = source.m_RasterState;
//	modelRotate = source.modelRotate;
//	modelScale = source.modelScale;
//	modelTranslate = source.modelTranslate;
//	normalTexture = source.normalTexture;
//	normalTextureBody = source.normalTextureBody;
//	normalTextureEye = source.normalTextureEye;
//	normalTextureWing = source.normalTextureWing;
//	objectName = source.objectName;
//	particleShader = source.particleShader;
//	rEyeMesh = source.rEyeMesh;
//	rasterDesc = source.rasterDesc;
//	reflectedEyeTextureFromObjectManager = source.reflectedEyeTextureFromObjectManager;
//	renderNumber = source.renderNumber;
//	shaderTag = source.shaderTag;
//	velocity = source.velocity;
//	wingMesh = source.wingMesh;
//	wingShader = source.wingShader;
//	t = source.t;
//	return *this;
//	// TODO: insert return statement here
//}

DragonFlyObject::~DragonFlyObject()
{
	try
	{
		wingMesh.Destroy();
		lEyeMesh.Destroy();
		rEyeMesh.Destroy();
		bodyMesh.Destroy();
		if (eyeShader != nullptr)delete eyeShader;
		if (wingShader != nullptr)delete wingShader;
		if (normalTextureBody != nullptr) normalTextureBody->Release();
		if (normalTextureWing != nullptr) normalTextureWing->Release();
		if (normalTextureEye != nullptr) normalTextureEye->Release();
		if (DepthStencilStateObject != nullptr) DepthStencilStateObject->Release();;
		if (g_pSamplerLinear != nullptr) g_pSamplerLinear->Release();
		if (g_pConstantBuffer != nullptr) g_pConstantBuffer->Release();
		for (unsigned int i = 0; i < collsionList.size(); i++)
		{
			delete collsionList[i];
		}
		if (m_RasterState != nullptr) m_RasterState->Release();
		delete particleShader;
	}
	catch (const std::exception&)
	{
	}
}

const std::string& DragonFlyObject::GetObjectTag()

{
	return animationShaderTag;
}

const XMFLOAT3& DragonFlyObject::GetTranslation()const
{
	return modelTranslate;
}

void DragonFlyObject::SetTranslation(XMFLOAT3 const& transValue)
{
	modelTranslate = transValue;
}

const XMFLOAT3& DragonFlyObject::GetScale()const
{
	return modelScale;
}

void DragonFlyObject::SetScale(XMFLOAT3 const& scaleValue)
{
	modelScale = scaleValue;
}

const XMFLOAT3& DragonFlyObject::GetRotation()const
{
	return modelRotate;
}

void DragonFlyObject::SetRoation(XMFLOAT3 const& rotValue)
{
	modelRotate = rotValue;
}

void DragonFlyObject::ResetModel()
{
	//reset all of the cams positions by storing te orginal positions
	modelTranslate = XMFLOAT3(0, 0, 0);
	modelRotate = XMFLOAT3(0, 0, 0);
	flying = false;
	disturbed = false;
	acsedning = false;
	renderNumber = 0;
	velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void DragonFlyObject::GetCam(std::string const& value)
{
	camTag = value;
}

void DragonFlyObject::SetReflectionTexture(ID3D11ShaderResourceView* const& value)
{
	reflectedEyeTextureFromObjectManager = value;
}

void DragonFlyObject::SetCurrentShader(ShaderClass* const shader)
{
	currentShader = shader;
}

bool DragonFlyObject::GetTakenOffBool()
{
	return disturbed;
}

//loads in all of the model parts, normal textures and creates all of the states
HRESULT DragonFlyObject::LoadModel(ID3D11Device* const pd3dDevice, ID3D11DeviceContext* const pd3dImmediateContext)
{
	HRESULT hr;
	modelTranslate = XMFLOAT3(0, 0, 0);
	modelRotate = XMFLOAT3(0, 0, 0);
	/*timeCur = 0;
	t = 0;
	timeStart = 0;*/
#pragma region loads in body parts and normal maps
	const WCHAR* const dragonFlyWingFileName = L"Tiny\\DragonFlyWings.sdkmesh";
	hr = wingMesh.Create(pd3dDevice, dragonFlyWingFileName);
	if (FAILED(hr))
		return hr;
	const WCHAR* const dragonFlylEyeFileName = L"Tiny\\DragonFlyLeftEye.sdkmesh";
	hr = lEyeMesh.Create(pd3dDevice, dragonFlylEyeFileName);
	if (FAILED(hr))
		return hr;
	const WCHAR* const dragonFlyReyeFileName = L"Tiny\\DragonFlyRightEye.sdkmesh";
	hr = rEyeMesh.Create(pd3dDevice, dragonFlyReyeFileName);
	if (FAILED(hr))
		return hr;

	const WCHAR* const dFlyBody = L"Tiny\\DragonFlyBody.sdkmesh";

	hr = bodyMesh.Create(pd3dDevice, dFlyBody);
	if (FAILED(hr))
		return hr;

	const WCHAR* const normalMapBody = L"Tiny\\NormalMapBody.dds";
	hr = CreateDDSTextureFromFile(pd3dDevice, normalMapBody, nullptr, &normalTextureBody);
	if (FAILED(hr))
		return hr;

	const WCHAR* const normalMapWing = L"Tiny\\NormalMapWings.dds";
	hr = CreateDDSTextureFromFile(pd3dDevice, normalMapWing, nullptr, &normalTextureWing);
	if (FAILED(hr))
		return hr;

	const WCHAR* const normalMapEye = L"Tiny\\NormalMapEyes.dds";
	hr = CreateDDSTextureFromFile(pd3dDevice, normalMapEye, nullptr, &normalTextureEye);
	if (FAILED(hr))
		return hr;
#pragma endregion

#pragma region setsup a sample state
	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr))
		return hr;

#pragma endregion

#pragma region sets up a constant buffer
	//set up constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;
#pragma endregion

#pragma region crerate depth stecnil state
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = true;
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

	hr = pd3dDevice->CreateDepthStencilState(&dsDesc, &DepthStencilStateObject);
	if (FAILED(hr))
		return hr;
#pragma endregion

#pragma region creates a raster state

	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = pd3dDevice->CreateRasterizerState(&rasterDesc, &m_RasterState);
	if (FAILED(hr))
		return hr;
#pragma endregion

#pragma region inilise and load the shaders that deal with the eye and wings

	const std::string refractedTag = "objectShader";
	const std::string dragonFlyShaderFile = "DragonFlyEffect.fx";
	eyeShader = new ShaderClass(dragonFlyShaderFile, "EyeVertexShader", "EyePixelShader", refractedTag);
	hr = eyeShader->LoadShader(pd3dDevice, pd3dImmediateContext);
	if (FAILED(hr))
		return hr;
	wingShader = new ShaderClass(dragonFlyShaderFile, "WingVertexShader", "WingPixelShader", refractedTag);
	hr = wingShader->LoadShader(pd3dDevice, pd3dImmediateContext);
	if (FAILED(hr))
		return hr;
	//load in the particle shader
#pragma endregion
	//loads in a collsion list. made into a list so more collsions could be added in future
	collsionList.push_back(new CollisionManager(XMFLOAT3(0, 0, 0), 3, 5, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0)));
	velocity = XMFLOAT3(0.0f, 0.0001f, 0.0f);

	m_Model = XMMatrixIdentity();
	return hr;
}

//this translates the position of the dragonfly and will eventually also check for collsions
void DragonFlyObject::UpdateModel(int const& renderNo, ID3D11Device* const pd3dDevice)
{
	//sets up the rastyer state according to a number pushed into the update method
	renderNumber = renderNo;
	switch (renderNumber)
	{
	case 0:
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		pd3dDevice->CreateRasterizerState(&rasterDesc, &m_RasterState);
		break;
	case 1:
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		pd3dDevice->CreateRasterizerState(&rasterDesc, &m_RasterState);
		break;

	default:
		break;
	}
	//disturbed = true;
	if (GetAsyncKeyState(122) && flying == false)
	{
		flying = true;
		disturbed = true;
		acsedning = true;
	}
	//set up the delta time for the flying and pathing. this doesnt work and i dont know why
	const float dt = ft.GetDeltaTime();
	if (flying == true)
	{
		//timer
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;

		t = (timeCur - timeStart) / 10000.0f;

		if (t > 0.4f && t < 0.7f)
		{
			velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			acsedning = false;
		}
		else if (t > 0.8f)
		{
			decending = true;
			velocity = XMFLOAT3(0.0f, -0.2f, 0.0f);
		}

		modelTranslate.x += velocity.x * dt;
		modelTranslate.y += velocity.y * dt;
		modelTranslate.z += velocity.z * dt;

		for (unsigned int i = 0; i < collsionList.size(); i++)
		{
			if (collsionList[i]->UpdateCollsions(GetTranslation(), 3) && decending == true)
			{
				velocity = collsionList[i]->getVelocity();
				disturbed = true;
				decending = false;
				flying = false;

				timeCur = 0;
				t = 0;
				timeStart = 0;
			}
		}
	}
}

//seprate model render method  for the rest of the body to separte the reflections from the eye
void DragonFlyObject::RenderBodyMethod(CDXUTSDKMesh const& mesh, ID3D11DeviceContext* const pd3dImmediateContext, ID3D11ShaderResourceView* const reflectedEyeTexture)const
{
	ID3D11Buffer* pVB;
	pVB = mesh.GetVB11(0, 0);
	UINT const Strides = static_cast<UINT>(mesh.GetVertexStride(0, 0));
	UINT const Offsets = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &pVB, &Strides, &Offsets);
	pd3dImmediateContext->IASetIndexBuffer(mesh.GetIB11(0), mesh.GetIBFormat11(0), 0);

	for (UINT subset = 0; subset < mesh.GetNumSubsets(0); ++subset)
	{
		const auto pSubset = mesh.GetSubset(0, subset);

		const auto PrimType = mesh.GetPrimitiveType11(static_cast<SDKMESH_PRIMITIVE_TYPE>(pSubset->PrimitiveType));
		pd3dImmediateContext->IASetPrimitiveTopology(PrimType);

		// Ignores most of the material information in them mesh to use only a simple shader

		const auto pDiffuseRV = mesh.GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
		pd3dImmediateContext->PSSetShaderResources(0, 1, &pDiffuseRV);
		pd3dImmediateContext->PSSetShaderResources(1, 1, &reflectedEyeTexture);
		pd3dImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

		pd3dImmediateContext->DrawIndexed(static_cast<UINT>(pSubset->IndexCount), 0, static_cast<UINT>(pSubset->VertexStart));
	}
}
/*this is a seprate method to render the eye to effectivly handle the reflections*/
void DragonFlyObject::RenderEyeMethod(CDXUTSDKMesh const& mesh, ID3D11DeviceContext* const pd3dImmediateContext, ID3D11ShaderResourceView* const reflectedEyeTexture)const
{
	UINT Strides;
	UINT Offsets;
	ID3D11Buffer* pVB;
	pVB = mesh.GetVB11(0, 0);
	Strides = static_cast<UINT>(mesh.GetVertexStride(0, 0));
	Offsets = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &pVB, &Strides, &Offsets);
	pd3dImmediateContext->IASetIndexBuffer(mesh.GetIB11(0), mesh.GetIBFormat11(0), 0);

	for (UINT subset = 0; subset < mesh.GetNumSubsets(0); ++subset)
	{
		const auto pSubset = mesh.GetSubset(0, subset);

		const auto PrimType = mesh.GetPrimitiveType11(static_cast<SDKMESH_PRIMITIVE_TYPE>(pSubset->PrimitiveType));
		pd3dImmediateContext->IASetPrimitiveTopology(PrimType);

		// Ignores most of the material information in them mesh to use only a simple shader
		if (camTag == "followEye")
		{
			pd3dImmediateContext->PSSetShaderResources(0, 1, &reflectedEyeTexture);
			pd3dImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
		}
		else
		{
			const auto pDiffuseRV = mesh.GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
			pd3dImmediateContext->PSSetShaderResources(0, 1, &pDiffuseRV);
			pd3dImmediateContext->PSSetShaderResources(1, 1, &reflectedEyeTexture);
			pd3dImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
		}
		pd3dImmediateContext->DrawIndexed(static_cast<UINT>(pSubset->IndexCount), 0, static_cast<UINT>(pSubset->VertexStart));
	}
}

void DragonFlyObject::RenderModel(ID3D11DeviceContext* const pd3dImmediateContext, XMMATRIX const& g_View, XMMATRIX const& g_Projection, XMVECTOR const& eyePos, LightManager* const lightManager)
{
	ConstantBuffer cb;
	static ULONGLONG timeStart = 0;

	if (disturbed == true)
	{
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;

		t = (timeCur - timeStart) / 10000.0f;
		if (t > 0.6f)
		{
			disturbed = false;
			timeCur = 0;
			timeStart = 0;
		}
	}

	const XMMATRIX mTranslate = XMMatrixTranslation(modelTranslate.x, modelTranslate.y, modelTranslate.z);
	//const XMMATRIX mScale = XMMatrixScaling(modelScale.x, modelScale.y, modelScale.z);
	const XMMATRIX mRotateX = XMMatrixRotationX(modelRotate.x);
	const XMMATRIX mRotateY = XMMatrixRotationY(modelRotate.y);
	const XMMATRIX mRotateZ = XMMatrixRotationZ(modelRotate.z);
	//m_Model = mScale;
	m_Model = mRotateX;
	m_Model *= mRotateY;
	m_Model *= mRotateZ;
	m_Model *= mTranslate;
	cb.modelMatrix = XMMatrixTranspose(m_Model);
	cb.viewMatrix = XMMatrixTranspose(g_View);
	cb.projectionMatrix = XMMatrixTranspose(g_Projection);
	cb.time = t;
	cb.lightPos = lightManager->GetPosition();
	cb.mEyeposition = eyePos;
	cb.vLightDir[0] = lightManager->GetLightDirection(0);
	cb.vLightDir[1] = lightManager->GetLightDirection(1);
	cb.vLightDir[2] = lightManager->GetLightDirection(2);
	cb.vLightDir[3] = lightManager->GetLightDirection(3);

	cb.vLightColor[0] = lightManager->GetLightColour(0);
	cb.vLightColor[1] = lightManager->GetLightColour(1);
	cb.vLightColor[2] = lightManager->GetLightColour(2);
	cb.vLightColor[3] = lightManager->GetLightColour(3);
	//set constant buffers

	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	pd3dImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	pd3dImmediateContext->OMSetDepthStencilState(DepthStencilStateObject, 1);
	pd3dImmediateContext->RSSetState(m_RasterState);

	RenderBodyMethod(bodyMesh, pd3dImmediateContext, normalTextureBody);

	//add an if statement here to check if flying == true and if so, set the animate shader
	if (flying)
	{
		const XMMATRIX mRotateZ = XMMatrixRotationY(13.5f);

		cb.rotationMatrix = XMMatrixTranspose(mRotateZ);
		pd3dImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
		pd3dImmediateContext->VSSetShader(wingShader->GetVertexShader(), nullptr, 0);
		pd3dImmediateContext->PSSetShader(wingShader->GetPixelShader(), nullptr, 0);
		pd3dImmediateContext->IASetInputLayout(wingShader->GetInputLayout());

		RenderBodyMethod(wingMesh, pd3dImmediateContext, normalTextureWing);

		pd3dImmediateContext->VSSetShader(currentShader->GetVertexShader(), nullptr, 0);
		pd3dImmediateContext->PSSetShader(currentShader->GetPixelShader(), nullptr, 0);
		pd3dImmediateContext->IASetInputLayout(currentShader->GetInputLayout());
	}
	else
	{
		RenderBodyMethod(wingMesh, pd3dImmediateContext, normalTextureWing);
	}

	//set the eye shader here, the tag
	if (camTag == "followEye")
	{
		//set the new shader
		//set the new input layout
		pd3dImmediateContext->VSSetShader(eyeShader->GetVertexShader(), nullptr, 0);
		pd3dImmediateContext->PSSetShader(eyeShader->GetPixelShader(), nullptr, 0);
		pd3dImmediateContext->IASetInputLayout(eyeShader->GetInputLayout());
		RenderEyeMethod(lEyeMesh, pd3dImmediateContext, reflectedEyeTextureFromObjectManager);
		RenderEyeMethod(rEyeMesh, pd3dImmediateContext, reflectedEyeTextureFromObjectManager);
		//set the old shader
		//set the old layout out
		pd3dImmediateContext->VSSetShader(currentShader->GetVertexShader(), nullptr, 0);
		pd3dImmediateContext->PSSetShader(currentShader->GetPixelShader(), nullptr, 0);
		pd3dImmediateContext->IASetInputLayout(currentShader->GetInputLayout());
	}
	else
	{
		RenderEyeMethod(lEyeMesh, pd3dImmediateContext, normalTextureEye);
		RenderEyeMethod(rEyeMesh, pd3dImmediateContext, normalTextureEye);
	}
}
#include "ProjectObject.h"

struct ConstantBuffer
{
	XMMATRIX modelMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMFLOAT3 lightPos;
	XMVECTOR mEyeposition;

	XMFLOAT4 vLightDir[4];
	XMFLOAT4 vLightColor[4];

	//float inRenderNumber;
};

ProjectObject::ProjectObject() :
	 normalTexture(nullptr), g_pSamplerLinear(nullptr), g_pConstantBuffer(nullptr), normalMapFilePath(""), DepthStencilStateObject(nullptr),
	shaderTag(""), m_RasterState(nullptr), renderNumber(0), reflectionTexture(nullptr), objectTag(""), modelFile(""), shader(nullptr)
{
}

/*instaead of the transformations, will need a filepath to the objects own normal map,
so the filepaths can be taken out of the load method
*/
ProjectObject::ProjectObject(std::string const &normalMap, std::string const &shaderName, std::string const &objectName, std::string const &mesh) :
	 normalTexture(nullptr), g_pSamplerLinear(nullptr), g_pConstantBuffer(nullptr), normalMapFilePath(normalMap), DepthStencilStateObject(nullptr),
	shaderTag(shaderName), m_RasterState(nullptr), renderNumber(0), reflectionTexture(nullptr), objectTag(objectName), modelFile(mesh), shader(nullptr)
{
}
ProjectObject::ProjectObject(const ProjectObject & source) :
	 normalTexture(source.normalTexture), g_pSamplerLinear(source.g_pSamplerLinear), g_pConstantBuffer(source.g_pConstantBuffer),
	normalMapFilePath(source.normalMapFilePath), DepthStencilStateObject(source.DepthStencilStateObject),
	shaderTag(source.shaderTag), m_RasterState(source.m_RasterState), renderNumber(source.renderNumber), reflectionTexture(source.reflectionTexture), objectTag(source.objectTag), modelFile(source.modelFile), shader(source.shader)
{
}
ProjectObject & ProjectObject::operator=(const ProjectObject & source)
{
	if (this == &source)
	{
		return *this;
	}
	    camTag = source.camTag;  
		m_Model = source.m_Model; 
		rasterDesc = source.rasterDesc;
		//modelFile == source.modelFile;
		normalTexture = source.normalTexture;
		g_pSamplerLinear = source.g_pSamplerLinear;
		g_pConstantBuffer = source.g_pConstantBuffer;
		normalMapFilePath = source.normalMapFilePath;
		DepthStencilStateObject = source.DepthStencilStateObject;
		shaderTag = source.shaderTag;
		m_RasterState = source.m_RasterState;
		renderNumber = source.renderNumber;
		reflectionTexture = source.reflectionTexture;
		objectTag =source.objectTag;
		shader = source.shader;
		g_Mesh = source.g_Mesh;
		modelFile = source.modelFile;
	return *this;
	// TODO: insert return statement here
}
ProjectObject::~ProjectObject()
{
	try
	{
		if (g_pSamplerLinear != nullptr) g_pSamplerLinear->Release();
		if (g_pConstantBuffer != nullptr) g_pConstantBuffer->Release();
		g_Mesh.Destroy();
	}
	catch (const std::exception&)
	{
			
	}
	
}
//definatly needs a normal texture to be set in the method
HRESULT ProjectObject::LoadModel(ID3D11Device* const pd3dDevice, ID3D11DeviceContext* const pd3dImmediateContext)
{
	/*modelScale = XMFLOAT3(1, 1, 1);
	modelTranslate = XMFLOAT3(0, 0, 0);
	modelRotate = XMFLOAT3(0,0,0);*/
	HRESULT hr;

	const std::wstring widestr = std::wstring(normalMapFilePath.begin(), normalMapFilePath.end());
	const WCHAR* const TextureFileName = widestr.c_str();
	hr = CreateDDSTextureFromFile(pd3dDevice, TextureFileName, nullptr, &normalTexture);
	if (FAILED(hr))
		return hr;

	const std::wstring modelFileNameWideString = std::wstring(modelFile.begin(), modelFile.end());
	const WCHAR* const modelFileName = modelFileNameWideString.c_str();
	hr = g_Mesh.Create(pd3dDevice, modelFileName);
	if (FAILED(hr))
		return hr;
	
#pragma region create sample state
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
	const std::string entag = "refractedEnviromentShader";
	if (shaderTag == entag)
		rasterDesc.CullMode = D3D11_CULL_BACK;
	else 
	    rasterDesc.CullMode = D3D11_CULL_FRONT;

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

	m_Model = XMMatrixIdentity();
	
	return hr;
}

void ProjectObject::UpdateModel(int const &renderNo, ID3D11Device* const pd3dDevice)
{
	renderNumber = renderNo;
	switch (renderNo)
	{
	case 0:
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		pd3dDevice->CreateRasterizerState(&rasterDesc, &m_RasterState);
		break;
	case 1 :
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		pd3dDevice->CreateRasterizerState(&rasterDesc, &m_RasterState);
		break;

	default:
		break;
	}
}

void ProjectObject::RenderModel(ID3D11DeviceContext* const pd3dImmediateContext, XMMATRIX const &g_View, XMMATRIX const &g_Projection, XMVECTOR const &eyePos, LightManager* const lightManager)
{
#pragma region update the constant buffers
	ConstantBuffer cb;
	cb.modelMatrix = XMMatrixTranspose(m_Model);
	cb.viewMatrix = XMMatrixTranspose(g_View);
	cb.projectionMatrix = XMMatrixTranspose(g_Projection);
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
//	cb.inRenderNumber = renderNumber;
#pragma endregion
	UINT Strides;
	UINT Offsets;
	ID3D11Buffer* pVB;
	pVB = g_Mesh.GetVB11(0, 0);
	Strides = static_cast<UINT>(g_Mesh.GetVertexStride(0, 0));
	Offsets = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &pVB, &Strides, &Offsets);
	pd3dImmediateContext->IASetIndexBuffer(g_Mesh.GetIB11(0), g_Mesh.GetIBFormat11(0), 0); 
	
	
	//set constant buffers
	pd3dImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	
	pd3dImmediateContext->OMSetDepthStencilState(DepthStencilStateObject, 1);
	pd3dImmediateContext->RSSetState(m_RasterState);
	
	//render the model
	for (UINT subset = 0; subset < g_Mesh.GetNumSubsets(0); ++subset)
	{
		const auto pSubset = g_Mesh.GetSubset(0, subset);

		const auto PrimType = g_Mesh.GetPrimitiveType11(static_cast<SDKMESH_PRIMITIVE_TYPE>(pSubset->PrimitiveType));
		pd3dImmediateContext->IASetPrimitiveTopology(PrimType);

		// Ignores most of the material information in them mesh to use only a simple shader
		if (shaderTag != "enviromentShader" && shaderTag != "refractedEnviromentShader")
		{
			const auto pDiffuseRV = g_Mesh.GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
		
			pd3dImmediateContext->PSSetShaderResources(0, 1, &pDiffuseRV);
			pd3dImmediateContext->PSSetShaderResources(1, 1, &normalTexture);
			pd3dImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
		}
		//else set the resource to be the texture that was passed into the instance via a set funtion
		pd3dImmediateContext->DrawIndexed(static_cast<UINT>(pSubset->IndexCount), 0, static_cast<UINT>(pSubset->VertexStart));
	}
}

void ProjectObject::ResetModel()
{
	renderNumber = 0;
}

const std::string& ProjectObject::GetObjectTag()
{
	return shaderTag;
}

void ProjectObject::SetReflectionTexture(ID3D11ShaderResourceView* const &value)
{
	reflectionTexture = value;
}

void ProjectObject::GetCam(std::string const &value)
{
	camTag = value;
}

void ProjectObject::SetCurrentShader(ShaderClass* const currentShader)
{
	shader = currentShader;
}

bool ProjectObject::GetTakenOffBool()
{
	return false;
}



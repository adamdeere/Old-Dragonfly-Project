#include "ObjectManager.h"
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

ObjectManager::ObjectManager() : graphicsPointer(nullptr), bar(nullptr), currentCam(nullptr),
lightManager(nullptr), camNumber(0), g_pRenderTargetTexture_RenTEX(nullptr), g_pRenderTargetView_RenTEX(nullptr),
g_pTextureRV_RenTEX(nullptr), dFlyObject(nullptr), camType(""), particleObject(nullptr), takenOff(false), renderNumber(0), keyPressed(false), timer(0)

{
}
ObjectManager::ObjectManager(GraphicsManager* const graphicsInstance) : graphicsPointer(graphicsInstance), bar(nullptr), currentCam(nullptr),
lightManager(nullptr), camNumber(0), g_pRenderTargetTexture_RenTEX(nullptr), g_pRenderTargetView_RenTEX(nullptr),
g_pTextureRV_RenTEX(nullptr), dFlyObject(nullptr), camType(""), particleObject(nullptr), takenOff(false), renderNumber(0), keyPressed(false), timer(0)
{
}

ObjectManager::ObjectManager(const ObjectManager& source) : graphicsPointer(source.graphicsPointer), bar(source.bar), currentCam(source.currentCam),
lightManager(source.lightManager), camNumber(source.camNumber), g_pRenderTargetTexture_RenTEX(source.g_pRenderTargetTexture_RenTEX), g_pRenderTargetView_RenTEX(source.g_pRenderTargetView_RenTEX),
g_pTextureRV_RenTEX(source.g_pTextureRV_RenTEX), dFlyObject(source.dFlyObject), camType(source.camType), particleObject(source.particleObject), takenOff(source.takenOff), renderNumber(source.renderNumber), keyPressed(source.keyPressed),
timer(source.timer)
{
}

ObjectManager& ObjectManager::operator=(const ObjectManager& source)
{
	if (this == &source)
	{
		return *this;
	}
	graphicsPointer = this->graphicsPointer;
	bar = source.bar;
	currentCam = source.currentCam;
	lightManager = source.lightManager;
	camNumber = source.camNumber;
	g_pRenderTargetTexture_RenTEX = source.g_pRenderTargetTexture_RenTEX;
	g_pRenderTargetView_RenTEX = source.g_pRenderTargetView_RenTEX;
	g_pTextureRV_RenTEX = source.g_pTextureRV_RenTEX;
	dFlyObject = source.dFlyObject;
	sphereMesh = source.sphereMesh;
	dragonFlyMesh = source.dragonFlyMesh;
	floorMesh = source.floorMesh;
	twigMesh = source.twigMesh;
	camList = source.camList;
	objectList = source.objectList;
	shaderList = source.shaderList;
	camType = source.camType;
	particleObject = source.particleObject;
	takenOff = source.takenOff;
	renderNumber = source.renderNumber;
	keyPressed = source.keyPressed;
	timer = source.timer;
	return *this;
}

ObjectManager::~ObjectManager()
{
	try
	{
		for (unsigned int i = 0; i < shaderList.size(); i++)
		{
			delete shaderList[i];
		}
		for (unsigned int i = 0; i < objectList.size(); i++)
		{
			delete objectList[i];
		}
		for (unsigned int i = 0; i < camList.size(); i++)
		{
			delete camList[i];
		}
		delete lightManager;
		delete particleObject;
		delete dFlyObject;
		/*if (dFlyObject != nullptr)
			delete dFlyObject;*/
	}
	catch (const std::exception&)
	{
	}
}

HRESULT ObjectManager::LoadModel(HWND const g_hWnd)
{
	HRESULT hr;
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	const UINT width = rc.right - rc.left;
	const UINT height = rc.bottom - rc.top;

#pragma region file parser
	ifstream fileIn("Text.txt");
	string name;
	while (fileIn.good())
	{
		getline(fileIn, name, ',');

		if (name == "shader")
		{
			string shaderFile;
			string vertexEntry;
			string pixelEntry;
			string shaderTag;
			getline(fileIn, shaderFile, ',');
			getline(fileIn, vertexEntry, ',');
			getline(fileIn, pixelEntry, ',');
			getline(fileIn, shaderTag, '\n');
			shaderList.push_back(new ShaderClass(shaderFile, vertexEntry, pixelEntry, shaderTag));
		}
		else if (name == "dragonFlyObject")
		{
			string tag;
			string objectName;

			getline(fileIn, tag, ',');
			getline(fileIn, objectName, '\n');
			dFlyObject = new DragonFlyObject(tag, objectName);
			objectList.push_back(dFlyObject);
		}
		else if (name == "envriomentObject")
		{
			string tag;
			getline(fileIn, tag, '\n');
			objectList.push_back(new EnviromentObject(tag));
		}
		else if (name == "projectObject")
		{
			string textureName;
			string tag;
			string objectName;
			string modelName;

			getline(fileIn, textureName, ',');
			getline(fileIn, tag, ',');
			getline(fileIn, objectName, ',');
			getline(fileIn, modelName, '\n');
			objectList.push_back(new ProjectObject(textureName, tag, objectName, modelName));
		}
	}
#pragma endregion

	particleObject = new ParticleObject(300);
	for (unsigned int i = 0; i < shaderList.size(); i++)
	{
		hr = shaderList[i]->LoadShader(graphicsPointer->getDevice(), graphicsPointer->getContext());
		if (FAILED(hr))
			return hr;
	}
#pragma region load all of the objects. this will be changed later

	for (unsigned int i = 0; i < objectList.size(); i++)
	{
		hr = objectList[i]->LoadModel(graphicsPointer->getDevice(), graphicsPointer->getContext());
		if (FAILED(hr))
			return hr;
	}
	particleObject->LoadParticle(graphicsPointer->getDevice());
	lightManager = new LightManager(graphicsPointer->getDriverType());
	lightManager->IntilizeLights();
	if (FAILED(hr))
		return hr;

#pragma endregion

	//set up a new render target view here like in the labs
	//maybe take out the target view from the graphics manager and place it here? more controll that way

	hr = TwInit(TW_DIRECT3D11, graphicsPointer->getDevice());
	if (FAILED(hr))
		return hr;

#pragma region sets up all of the ant tweak stuff
	bar = TwNewBar("TweakBar");

	TwAddVarRO(bar, "time", TW_TYPE_FLOAT, &timer, " label='time' ");
	TwAddVarCB(bar, "Translate fly", TW_TYPE_DIR3F, nullptr, DragonFlyObject::GetTranslationCallback, dFlyObject, "group=fly key=w");
	TwAddVarCB(bar, "rotate fly", TW_TYPE_DIR3F, DragonFlyObject::SetRotationCallback, DragonFlyObject::GetRotationCallback, dFlyObject, "group=fly key=w");
	TwAddVarCB(bar, "translate light", TW_TYPE_DIR3F, LightManager::SetTranslationCallback, LightManager::GetTranslationback, lightManager, "group=light key=w");
	TwAddVarRW(bar, "current cam", TW_TYPE_STDSTRING, &camType, "");

#pragma endregion

#pragma region sets up a render target view for rendering to a texture

	D3D11_TEXTURE2D_DESC renderableTextureDesc_RenTEX;
	ZeroMemory(&renderableTextureDesc_RenTEX, sizeof(renderableTextureDesc_RenTEX));
	renderableTextureDesc_RenTEX.Width = width;
	renderableTextureDesc_RenTEX.Height = height;
	renderableTextureDesc_RenTEX.MipLevels = 1;
	renderableTextureDesc_RenTEX.ArraySize = 1;
	renderableTextureDesc_RenTEX.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderableTextureDesc_RenTEX.SampleDesc.Count = 1;
	renderableTextureDesc_RenTEX.Usage = D3D11_USAGE_DEFAULT;
	renderableTextureDesc_RenTEX.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderableTextureDesc_RenTEX.CPUAccessFlags = 0;
	renderableTextureDesc_RenTEX.MiscFlags = 0;

	hr = graphicsPointer->getDevice()->CreateTexture2D(&renderableTextureDesc_RenTEX, nullptr, &g_pRenderTargetTexture_RenTEX);
	if (FAILED(hr))
		return hr;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = renderableTextureDesc_RenTEX.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = graphicsPointer->getDevice()->CreateRenderTargetView(g_pRenderTargetTexture_RenTEX, nullptr, &g_pRenderTargetView_RenTEX);
	if (FAILED(hr))
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = renderableTextureDesc_RenTEX.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = graphicsPointer->getDevice()->CreateShaderResourceView(g_pRenderTargetTexture_RenTEX, &shaderResourceViewDesc, &g_pTextureRV_RenTEX);
	if (FAILED(hr))
		return hr;

#pragma endregion

	const XMVECTOR g_Eye = XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f);
	const XMVECTOR eyePosition = XMVectorSet(-6.0f, 4.5f, -3.0f, 1.0f);
	const XMVECTOR At = XMVectorSet(0.0f, -1.0f, -1.0f, 0.0f);
	const XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR insideEye = XMVectorSet(0.0f, 0.5f, -3.0f, 1.0f);
	const XMVECTOR inAt = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR outAt = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMMATRIX g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / static_cast<FLOAT>(height), 0.01f, 1000.0f);

	const std::string followCamString = "follow";
	const std::string followEyeCamString = "followEye";
	const std::string insideCamString = "inside";
	const std::string outsideCam = "outside";

	//these two will be set in the update method anyways
	const XMMATRIX View = XMMatrixLookAtLH(g_Eye, At, Up);
	camList.push_back(new CameraManager(followCamString, g_Eye, At, Up, View, g_Projection));

	camList.push_back(new CameraManager(followEyeCamString, g_Eye, At, Up, View, g_Projection));

	const XMMATRIX g_View = XMMatrixLookAtLH(insideEye, inAt, Up);
	camList.push_back(new CameraManager(insideCamString, insideEye, At, Up, g_View, g_Projection));
	const XMMATRIX h_View = XMMatrixLookAtLH(eyePosition, At, Up);
	camList.push_back(new CameraManager(outsideCam, eyePosition, outAt, Up, h_View, g_Projection));

	//these two will need to be set in there correct position

	camNumber = 0;
	return hr;
}
//update the camrea position and all of the render states
void ObjectManager::UpdateModel()
{
	static ULONGLONG timeStart = 0;
	ULONGLONG const timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;

	timer = (timeCur - timeStart) / 10000.0f;
	//keys f1 to f4 change camera positions
	//key f5 changes the render state
	//key f12 changes the bool to say the dragonfly is flying
	//key for f1
	if (GetAsyncKeyState(112))
		camNumber = 0;
	//key for f2
	if (GetAsyncKeyState(113))
		camNumber = 1;
	//key for f3
	if (GetAsyncKeyState(114))
		camNumber = 2;

	//key for f4
	if (GetAsyncKeyState(115))
		camNumber = 3;

	if (GetAsyncKeyState(116) && keyPressed == false)
	{
		keyPressed = true;
		renderNumber++;
		if (renderNumber > 1)
		{
			renderNumber = 0;
		}
	}
	if (GetAsyncKeyState(116) == false)
	{
		keyPressed == false;
	}

	if (GetAsyncKeyState('R'))
	{
		for (unsigned int i = 0; i < objectList.size(); i++)
		{
			objectList[i]->ResetModel();
		}
		for (unsigned int i = 0; i < camList.size(); i++)
		{
			camList[i]->ResetCams();
		}
		currentCam->ResetCams();
		renderNumber = 0;
		keyPressed = false;
	}

	switch (camNumber)
	{
	case 0:
		currentCam = camList[0];
		camType = currentCam->GetName();
		break;
	case 1:
		currentCam = camList[1];
		camType = currentCam->GetName();
		break;
	case 2:
		currentCam = camList[2];
		camType = currentCam->GetName();
		break;
	case 3:
		currentCam = camList[3];
		camType = currentCam->GetName();
		break;
	default:
		currentCam = camList[0];
		camType = currentCam->GetName();
		break;
	}

	currentCam->UpdateCamPosition(dFlyObject);

	for (unsigned int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->UpdateModel(renderNumber, graphicsPointer->getDevice());
	}
	lightManager->UpdateLights();
}

void ObjectManager::RenderModel()
{
	//this method essentially pre renders a texture that will later be used on the dragon flyes eyes should it
	//need to.
	graphicsPointer->getContext()->ClearRenderTargetView(g_pRenderTargetView_RenTEX, Colors::MidnightBlue);

	// Clear the depth buffer to 1.0 (max depth)
	graphicsPointer->getContext()->ClearDepthStencilView(graphicsPointer->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	graphicsPointer->getContext()->OMSetRenderTargets(1, &g_pRenderTargetView_RenTEX, graphicsPointer->getDepthStencilView());
	for (unsigned int i = 0; i < shaderList.size(); i++)
	{
		graphicsPointer->getContext()->VSSetShader(shaderList[i]->GetVertexShader(), nullptr, 0);
		graphicsPointer->getContext()->PSSetShader(shaderList[i]->GetPixelShader(), nullptr, 0);
		graphicsPointer->getContext()->IASetInputLayout(shaderList[i]->GetInputLayout());
		const std::string tag = shaderList[i]->GetShaderTag();
		for (unsigned int j = 0; j < objectList.size(); j++)
		{
			const std::string objectTag = objectList[j]->GetObjectTag();
			if (tag == objectTag)
			{
				objectList[j]->RenderModel(graphicsPointer->getContext(), currentCam->GetView(), currentCam->GetProjection(), currentCam->GetEye(), lightManager);
			}
		}
	}
	//when we get here we render the actual scene as it appears to the viewer. one slight change is we pass in the rendered
	//texture to the apprite class if it so requires
	// clear the backbuffer
	graphicsPointer->getContext()->ClearRenderTargetView(graphicsPointer->getRenderTargetView(), Colors::MidnightBlue);
	//set the render target view to be the above
	// Clear the depth buffer to 1.0 (max depth)
	graphicsPointer->getContext()->ClearDepthStencilView(graphicsPointer->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	ID3D11RenderTargetView* const rtView = graphicsPointer->getRenderTargetView();
	graphicsPointer->getContext()->OMSetRenderTargets(1, &rtView, graphicsPointer->getDepthStencilView());

	for (unsigned int i = 0; i < shaderList.size(); i++)
	{
		graphicsPointer->getContext()->VSSetShader(shaderList[i]->GetVertexShader(), nullptr, 0);
		graphicsPointer->getContext()->PSSetShader(shaderList[i]->GetPixelShader(), nullptr, 0);
		graphicsPointer->getContext()->IASetInputLayout(shaderList[i]->GetInputLayout());
		const std::string tag = shaderList[i]->GetShaderTag();
		for (unsigned int j = 0; j < objectList.size(); j++)
		{
			const std::string objectTag = objectList[j]->GetObjectTag();
			if (objectTag == "dragonFly")
			{
			}
			if (tag == objectTag)
			{
				objectList[j]->SetCurrentShader(shaderList[i]);
				objectList[j]->GetCam(currentCam->GetName());
				objectList[j]->SetReflectionTexture(g_pTextureRV_RenTEX);
				objectList[j]->RenderModel(graphicsPointer->getContext(), currentCam->GetView(), currentCam->GetProjection(), currentCam->GetEye(), lightManager);
				takenOff = objectList[j]->GetTakenOffBool();
			}
			else if (tag == "particleShader")
			{
				if (takenOff == true)
				{
					particleObject->RenderParticle(graphicsPointer->getContext(), currentCam->GetView(), currentCam->GetProjection(), shaderList[i]->GetInputLayout());
				}
			}
		}
	}
	TwDraw();

	graphicsPointer->getSwapChain()->Present(0, 0);
}
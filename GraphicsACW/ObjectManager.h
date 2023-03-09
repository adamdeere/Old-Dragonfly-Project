#pragma once
#include "ProjectObject.h"
#include "GraphicsManager.h"
#include "ShaderClass.h"
#include "EnviromentObject.h"
#include "DragonFlyObject.h"
#include <vector>
#include <string>
#include <AntTweakBar.h>
#include <d3d11.h>
#include "CameraManager.h"
#include "ParticleObject.h"

using namespace DirectX;
class ObjectManager
{
public:
	explicit ObjectManager();
	explicit ObjectManager(GraphicsManager* const graphicsInstance);
	ObjectManager(const ObjectManager&  source);
	ObjectManager& operator = (const ObjectManager&  source);

	~ObjectManager();
	HRESULT LoadModel(HWND const g_hWnd);
	void UpdateModel();
	void RenderModel();
private:
	CDXUTSDKMesh  sphereMesh;
	CDXUTSDKMesh  dragonFlyMesh;
	CDXUTSDKMesh  floorMesh;
	CDXUTSDKMesh  twigMesh;
	
	GraphicsManager* graphicsPointer;

	TwBar* bar;
	
	std::vector<ShaderClass*> shaderList;
	std::vector<ProjectObject*> objectList;
	std::vector<CameraManager*> camList;
	CameraManager* currentCam;
	LightManager* lightManager;
	//dunno if i need these yet
	
	int camNumber;
	
	//can possibly move to the graphics manager
	ID3D11Texture2D* g_pRenderTargetTexture_RenTEX;
	ID3D11RenderTargetView* g_pRenderTargetView_RenTEX;
	ID3D11ShaderResourceView* g_pTextureRV_RenTEX;
	DragonFlyObject* dFlyObject;
	std::string camType;
	ParticleObject* particleObject;
	 bool takenOff;
	 int renderNumber;
	 bool keyPressed;
	 float timer;
};


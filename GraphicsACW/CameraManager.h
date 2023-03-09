#pragma once
#include <directxmath.h>
#include <windows.h>
#include <vector>
#include <string>
#include <d3d11.h>
#include "DragonFlyObject.h"
using namespace DirectX;
class CameraManager
{
public:
	CameraManager();
	CameraManager(const std::string &name, XMVECTOR const &g_Eye, XMVECTOR const &At, XMVECTOR const &Up, XMMATRIX const &g_View, XMMATRIX const &g_Projection);
	CameraManager(const CameraManager&  source);
	CameraManager& operator= (const CameraManager&  source);
	~CameraManager();

	 const XMVECTOR& const GetEye() const;
	 const XMMATRIX& const GetView()const;
	 const XMMATRIX& const GetProjection()const;
	CameraManager* const GetCurrentCam();
	 const std::string& const GetName() const ;
	void UpdateCamPosition(const DragonFlyObject* const dFly);
	const void ResetCams();
private:

	const std::string camName;
	XMVECTOR eyePosition;
	XMVECTOR atPosition;
	XMVECTOR upPosition;
	XMMATRIX viewPosition;
	XMMATRIX viewProjection;

	XMVECTOR originalEyePosition;
	XMVECTOR originalAtPosition;
	XMVECTOR originalUpPosition;
	XMMATRIX originalViewPosition;
	XMMATRIX originalWiewProjection;
	
};


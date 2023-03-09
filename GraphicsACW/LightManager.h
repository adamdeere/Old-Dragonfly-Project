#pragma once
#include <AntTweakBar.h>
#include <d3d11.h>
#include <windows.h>
#include "DXUT.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include <string>
#include "Timer.h"

using namespace DirectX;

class LightManager
{
public:
	explicit LightManager(D3D_DRIVER_TYPE const driverType);
	LightManager(const LightManager&  source);
	LightManager& operator= (const LightManager&  source);
	~LightManager();
	void IntilizeLights();
	void UpdateLights();
	//this is only so I can have a visual reresentaion of the lights. can be toggled on or off via
	//the ant tweak bar
	void RenderLights()const;
	const XMFLOAT3& GetPosition()const;
	void SetPosition(XMFLOAT3 const &lightValue);

	XMFLOAT4& GetLightDirection(int const &index);
	XMFLOAT4& GetLightColour(int const &index);
	XMFLOAT3& GetSpotPosition(int const &index);
#pragma region call backs to set the lights positioning and colour
	static void TW_CALL LightManager::SetTranslationCallback(const void * const value, void* const clientData)
	{
		static_cast<LightManager*>(clientData)->SetPosition(*static_cast<const XMFLOAT3*>(value));   
	}

	static void TW_CALL LightManager::GetTranslationback(void* const value, void* const clientData)
	{
		*static_cast<XMFLOAT3*>(value) = static_cast<LightManager*>(clientData)->GetPosition();
	}
	
#pragma endregion

private:

    XMFLOAT3 ambiantLightPosition;
	XMFLOAT4 lightDirection[4];
	XMFLOAT4 lightColour[4];
	XMFLOAT3 spotLightPosition[4];
	D3D_DRIVER_TYPE g_DriverType;
	Timer* timer;

};


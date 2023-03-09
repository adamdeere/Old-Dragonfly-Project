#include "LightManager.h"



LightManager::LightManager(D3D_DRIVER_TYPE const driverType) : g_DriverType(driverType), timer(nullptr)
{
}


LightManager::~LightManager()
{
	delete timer;
}

void LightManager::IntilizeLights()
{
	ambiantLightPosition = XMFLOAT3(0.0f, 1.0f, 1.0f);
	lightColour[0] = XMFLOAT4(0.0f, 0.1f, 0.0f, 0.1f);
	lightColour[1] = XMFLOAT4(0.0f, 0.2f, 0.0f, 0.0f);
	lightColour[2] = XMFLOAT4(0.5f, 0.1f, 0.2f, 1.0f);
	lightColour[3] = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.4f);
	
	lightDirection[0] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	lightDirection[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	lightDirection[2] = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	lightDirection[3] = XMFLOAT4(1.0f, -1.0f, 0.0f, 0.0f);
	timer = new Timer();
	
}

void LightManager::UpdateLights()
{
	/*static float t = 0.0f;
	if (g_DriverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += static_cast<float>(XM_PI * 0.0125f);
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG const timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 10000.0f;
	}*/
	UINT lightArraysize = ARRAYSIZE(lightDirection);
	
	const float t = timer->GetDeltaTime();
	for (unsigned int i = 1; i < lightArraysize; i++)
	{
		XMMATRIX const mRotate = XMMatrixRotationY(-0.02 * t);
		XMVECTOR  vLightDir = XMLoadFloat4(&lightDirection[i]);
		vLightDir = XMVector3Transform(vLightDir, mRotate);
		XMStoreFloat4(&lightDirection[i], vLightDir);
	}
	
}

void LightManager::RenderLights()const
{
}

const XMFLOAT3& LightManager::GetPosition()const
{
	return ambiantLightPosition;
}

void LightManager::SetPosition(XMFLOAT3 const &lightValue)
{
	ambiantLightPosition = lightValue;
}

XMFLOAT4& LightManager::GetLightDirection(int const &index)
{
	return lightDirection[index];
}

XMFLOAT4& LightManager::GetLightColour(int const &index)
{
	return lightColour[index];
}

XMFLOAT3& LightManager::GetSpotPosition(int const &index)
{
	return spotLightPosition[index];
}



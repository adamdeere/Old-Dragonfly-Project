#include "CameraManager.h"

CameraManager::CameraManager() : eyePosition(), atPosition(), upPosition(), originalEyePosition(), originalAtPosition(), originalUpPosition(), originalViewPosition(),
originalWiewProjection()

{
}
/*will need to keep hold of the original positions for the reset
will also need to change the positions of the follow and eye. else change the position of the cam via
key presses
*/
CameraManager::CameraManager(const std::string& name, XMVECTOR const& g_Eye, XMVECTOR const& At, XMVECTOR const& Up, XMMATRIX const& g_View, XMMATRIX const& g_Projection) :
	camName(name), eyePosition(g_Eye), atPosition(At), upPosition(Up), viewPosition(g_View), viewProjection(g_Projection), originalEyePosition(g_Eye), originalAtPosition(At), originalUpPosition(Up), originalViewPosition(g_View),
	originalWiewProjection(g_Projection)
{
}

CameraManager::~CameraManager()
{
}
const XMMATRIX& const CameraManager::GetProjection() const
{
	return viewProjection;
}
const XMVECTOR& const CameraManager::GetEye() const
{
	return eyePosition;
}

const XMMATRIX& const CameraManager::GetView() const
{
	return viewPosition;
}

CameraManager* const CameraManager::GetCurrentCam()
{
	return this;
}

const std::string& const CameraManager::GetName() const
{
	return camName;
}

void CameraManager::UpdateCamPosition(const DragonFlyObject* const dFly)
{
	//checks to see if the camera should be tracking and sets the positions accordin to the dragonfly
	//this would work in part, except for the bug with the delta time and the dragonfly (see dragon fly)
	if (camName == "follow")
	{
		const XMFLOAT3 dFlyPosition = dFly->GetTranslation();
		const XMVECTOR t = XMVectorSet(dFlyPosition.x, dFlyPosition.y, dFlyPosition.z, 0.0f);
		eyePosition = XMVectorSet(dFlyPosition.x, dFlyPosition.y + 1.3f, dFlyPosition.z, 1.0f) + t;
		const XMVECTOR At = XMVectorSet(dFlyPosition.x, dFlyPosition.y, dFlyPosition.z - 1.0f, 0.0f);
		const XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		viewPosition = XMMatrixLookAtLH(eyePosition, At, Up);
	}
	else if (camName == "followEye")
	{
		eyePosition = XMVectorSet(dFly->GetTranslation().x - 0.5f, dFly->GetTranslation().y, dFly->GetTranslation().z - 1.5f, 1.0f);
		const XMVECTOR At = XMVectorSet(-15.0f, 18.0f, -18.0f, 15.0f);
		const XMVECTOR Up = XMVectorSet(0, 1, 0.0f, 0);
		viewPosition = XMMatrixLookAtLH(eyePosition, At, Up);
		viewPosition *= XMMatrixRotationY(-0.5f);
	}
	else
	{
#pragma region update the eye position needs work
		//updates the cam positions according to the key presses if it is in the correct camera
		float x = XMVectorGetX(eyePosition);
		float y = XMVectorGetY(eyePosition);
		const float z = XMVectorGetZ(eyePosition);

		float atx = XMVectorGetX(atPosition);
		float aty = XMVectorGetY(atPosition);
		if (GetAsyncKeyState('S'))
		{
			viewPosition *= XMMatrixRotationX(-0.01f);
		}
		if (GetAsyncKeyState('W'))
		{
			viewPosition *= XMMatrixRotationX(0.01f);
		}
		if (GetAsyncKeyState('A'))
		{
			viewPosition *= XMMatrixRotationY(0.01f);
		}
		if (GetAsyncKeyState('D'))
		{
			viewPosition *= XMMatrixRotationY(-0.01f);
		}

		if (GetAsyncKeyState('S') && GetAsyncKeyState(VK_LCONTROL))
		{
			y -= 0.01f;
			aty -= 0.01f;
			atPosition = XMVectorSet(XMVectorGetX(atPosition), XMVectorGetY(atPosition), XMVectorGetZ(atPosition) + aty, XMVectorGetW(atPosition));
			upPosition = XMVectorSet(XMVectorGetX(upPosition), XMVectorGetY(upPosition), XMVectorGetZ(upPosition), XMVectorGetW(upPosition));
			eyePosition = XMVectorSet(x, y, z, 0.0f);
			viewPosition = XMMatrixLookAtLH(eyePosition, atPosition, upPosition);
		}
		if (GetAsyncKeyState('W') && GetAsyncKeyState(VK_LCONTROL))
		{
			y += 0.01f;
			aty += 0.01f;
			atPosition = XMVectorSet(XMVectorGetX(atPosition), XMVectorGetY(atPosition), XMVectorGetZ(atPosition) + y, XMVectorGetW(atPosition));
			upPosition = XMVectorSet(XMVectorGetX(upPosition), XMVectorGetY(upPosition), XMVectorGetZ(upPosition), XMVectorGetW(upPosition));
			eyePosition = XMVectorSet(x, y, z, 0.0f);
			viewPosition = XMMatrixLookAtLH(eyePosition, atPosition, upPosition);
		}
		if (GetAsyncKeyState('A') && GetAsyncKeyState(VK_LCONTROL))
		{
			x -= 0.01f;
			atx -= 0.01f;
			atPosition = XMVectorSet(XMVectorGetX(atPosition), XMVectorGetY(atPosition), XMVectorGetZ(atPosition), XMVectorGetW(atPosition) + atx);
			upPosition = XMVectorSet(XMVectorGetX(upPosition), XMVectorGetY(upPosition), XMVectorGetZ(upPosition), XMVectorGetW(upPosition));
			eyePosition = XMVectorSet(x, y, z, 0.0f);
			viewPosition = XMMatrixLookAtLH(eyePosition, atPosition, upPosition);
		}
		if (GetAsyncKeyState('D') && GetAsyncKeyState(VK_LCONTROL))
		{
			x += 0.01f;
			atx += 0.01f;
			atPosition = XMVectorSet(XMVectorGetX(atPosition), XMVectorGetY(atPosition), XMVectorGetZ(atPosition), XMVectorGetW(atPosition) + atx);
			upPosition = XMVectorSet(XMVectorGetX(upPosition), XMVectorGetY(upPosition), XMVectorGetZ(upPosition), XMVectorGetW(upPosition));
			eyePosition = XMVectorSet(x, y, z, 0.0f);
			viewPosition = XMMatrixLookAtLH(eyePosition, atPosition, upPosition);
		}

#pragma endregion
	}
}
//simple reset method
const void CameraManager::ResetCams()
{
	viewPosition = XMMatrixLookAtLH(originalEyePosition, originalAtPosition, originalUpPosition);
}
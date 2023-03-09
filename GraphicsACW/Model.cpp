#include "Model.h"


Model::Model()
{
}
Model::~Model()
{
}
HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* const pd3dDevice, CDXUTSDKMesh &g_Mesh)
{
	HRESULT hr;
    //V_RETURN(g_Mesh.Create(pd3dDevice, L"Tiny\\texturedCube.sdkmesh"));
	hr = g_Mesh.Create(pd3dDevice, L"Tiny\\texturedCube.sdkmesh");
	if (FAILED(hr))
		return hr;

	return hr;
}
HRESULT Model::InitilizeModel(ID3D11Device* const pd3dDevice)
{
	HRESULT hr = S_OK;

	//hr = OnD3D11CreateDevice(pd3dDevice, g_Mesh);
	//hr = g_Mesh.Create(pd3dDevice, L"Tiny\\texturedCube.sdkmesh");
	if (FAILED(hr))
		return hr;
	
	return hr;
}

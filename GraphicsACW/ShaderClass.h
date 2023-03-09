#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <string>
class ShaderClass
{
public:
	
	ShaderClass(const std::string &eFileName, const std::string &vertEntryPoint, const std::string &pixelEntryPoint, const std::string &tag);
	ShaderClass(const ShaderClass&  source);
    ShaderClass& operator= (const ShaderClass&  source);
	~ShaderClass();

	HRESULT LoadShader(ID3D11Device* const g_pd3dDevice, ID3D11DeviceContext* const g_pImmediateContext);

	ID3D11VertexShader* GetVertexShader()const;
	ID3D11PixelShader*  GetPixelShader()const;
    ID3D11InputLayout*  GetInputLayout();
   const std::string&  GetShaderTag() const;

private:
	HRESULT CompileShaderFromFile(const WCHAR* const szFileName, LPCSTR const szEntryPoint, LPCSTR const szShaderModel, ID3DBlob** const ppBlobOut)const;
	 const std::string effectFileString;
	 const std::string effectVertexString;
	 const std::string effectPixelString;
	 const std::string shaderTag;
	ID3D11VertexShader* g_pVertexShader;
    ID3D11PixelShader*  g_pPixelShader;
	ID3D11InputLayout*  g_pVertexLayout;
};


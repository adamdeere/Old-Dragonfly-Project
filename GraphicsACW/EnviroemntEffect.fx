//--------------------------------------------------------------------------------------
// File: Enviroment.fx
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 eyePosition;
}
struct PS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
};
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float3 viewDir : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(PS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.viewDir = input.Pos;
	output.Norm = input.Norm;

	float4 skyPos = float4(eyePosition.xyz, 0.0) + input.Pos;

	output.Pos = mul(skyPos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Norm = input.Norm;
	return output;
}

TextureCube txSkyColor      : register(t0);
SamplerState txSkySampler : register(s0);

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	return txSkyColor.Sample(txSkySampler, input.viewDir);
}



//-------------------------------------------------------------------------------------
//shiney object vertex shader
//-------------------------------------------------------------------------------------
VS_OUTPUT RefractedObjectVS(PS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	
	output.viewDir = eyePosition.xyz;
	output.Norm = input.Norm;

	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	return output;
}
//-------------------------------------------------------------------------------------
//shiney object pixel shader
//-------------------------------------------------------------------------------------
float4 RefractedObjectPS(VS_OUTPUT input) : SV_Target
{
	float3 reflViewDir = reflect(-input.viewDir, input.Norm);
	float3 refrViewDir = refract(-input.viewDir, input.Norm, 0.1);

	float4 skyColor = txSkyColor.Sample(txSkySampler, reflViewDir);
	float4 refrColor = txSkyColor.Sample(txSkySampler, refrViewDir);
	
	return  0.5 * skyColor + 0.5 * refrColor;

}



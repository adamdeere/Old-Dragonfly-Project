Texture2D txWoodColor : register(t0);
SamplerState txWoodsamSampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float time;
	float3 particleSize;
}
struct PS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORDS;
};
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float lifeCycle : TEXCOORD1;
};
VS_OUTPUT ParticleVertexEntry(PS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Tex = input.Tex;

	//billboarding
	float3 viewLeft = View._11_21_31;
	float3 viewUp = View._12_22_32;

	float4 inPos = input.Pos;
	inPos.xyz = inPos.x * viewLeft + inPos.y * viewUp;

	//inPos.xyz *= 2.5;

	float param = input.Pos.z;

	float speed = 0.1;
	float T = frac(param + speed * time);
	float systemHeight = 13;
	float radius = 10;

	//this will be animated from here
	float3 pos;
	pos.x = radius * T * cos(300.0 * param);
	pos.z = radius * T * sin(300.0 * param);
	pos.y = T * systemHeight;

	float fadeRate = 0.9;
	output.lifeCycle = 1.0 - (T / fadeRate);
	inPos.xyz += pos;
	output.Pos = mul(inPos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	return output;
}

float4 ParticlePixelEntry(VS_OUTPUT input) : SV_TARGET
{
	float4 woodColor = txWoodColor.Sample(txWoodsamSampler, input.Tex);
	float4 sandColour = float4(1.0, 1.0, 0.0, 0.0);
	woodColor = woodColor * (1.0 - smoothstep(0.0, 0.5, length(input.Tex - 0.5)));
	return woodColor * input.lifeCycle;
}
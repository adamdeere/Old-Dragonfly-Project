Texture2D modelTexture : register(t0);
Texture2D modelTextureNormal : register(t1);
SamplerState modelSampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	//turn this into an array of all the lights
	float time;
	float3 lightPosition;

	float4 inEyePosition;

	float4 vLightDir[4];
	float4 vLightColor[4];

	matrix Rotation;
}
struct PS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORDS;
	float3 Norm : NORMAL;
	float3 Tang : TANGENTS;
	float3 BiNorm : BINORMALS;
};
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	//turn this into an array
	float3 viewDirInTang : TEXCOORD1;
	float3 lightDirInTang[4] : TEXCOORD2;
};

VS_OUTPUT DragonFlyVertexEntry(PS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 viewDirW = inEyePosition - input.Pos.xyz;
	//turn lightDirW into an array and loop through
	float3 lightDirW[4];// = lightPosition - input.Pos.xyz;

	for (int i = 0; i < 4; i++)
	{
		lightDirW[i] = vLightDir[i] - input.Pos;
	}
	float3 rotatedNormals = mul(input.Norm, World);

	float3 N = normalize(rotatedNormals);
	float3 T = normalize(input.Tang);
	float3 B = normalize(input.BiNorm);

	float3x3 mat2Tang = float3x3 (T, B, N);
	output.viewDirInTang = mul(mat2Tang, viewDirW);

	//turn these two into arrays
	for (int i = 0; i < 4; i++)
	{
		output.lightDirInTang[i] = mul(mat2Tang, lightDirW[i]);
	}
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	return output;
}
float4 DragonFlyPixelEntry(VS_OUTPUT input) : SV_TARGET
{
	float4 finalColour = 0;
	/*this is where height map will be added if i have time
	float Height = modelTextureHeight.Sample(modelSampler, input.Tex).x;
	Height = 1 * Height - 0.5;
	float2 TexCorrected = input.Tex + Height * V.xy;*/

	//this is where ill set up flags and tags for render states. next job
	float4 NormalColour = modelTextureNormal.Sample(modelSampler, input.Tex);
	float4 textureColour = modelTexture.Sample(modelSampler, input.Tex);

	float3 Normal = normalize(2.0 * NormalColour.xyz - 1.0);

	//will i need these since light colour is coming through the gpu?
	float4 C = float4(0.1, 0.1, 0.1, 0.1);
	float3 V = normalize(input.viewDirInTang - input.Pos.xyz);

	//these four will be in the for loop
	for (int i = 0; i < 4; i++)
	{
		float3 R = reflect(-input.lightDirInTang[i], Normal);
		float spec = pow(max(1.0, dot(V, R)), 0.0);
		float diff = saturate(dot(input.lightDirInTang[i], Normal));
		finalColour += diff + spec * vLightColor[i];
	}
	finalColour = finalColour + C;
	float4 colour = float4(0.1,0.1,0.1,0.1);
	//return the final colour which will hopefully be all of the lights
	return finalColour * textureColour;
}

//have a pass that will texture the eye
VS_OUTPUT EyeVertexShader(PS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 inPos = input.Pos;

	//inPos *= Rotation;

	output.Pos = mul(inPos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	return output;
}

float4 EyePixelShader(VS_OUTPUT input) : SV_TARGET
{
	float4 textureColour = modelTexture.Sample(modelSampler, input.Tex);
	return textureColour;
}

//here have a pass that will animate the wings
VS_OUTPUT WingVertexShader(PS_INPUT input)
{
	//here we will animate the wings and then clip it into the space
	VS_OUTPUT output = (VS_OUTPUT)0;

	//float step = input.Pos.y;
	//if (step < 0)
	//{
	//	step *= -1;
	//}
	//input.Pos.y = lerp(input.Pos.y, input.Pos.y + cos(step), time);

	//float4 inPos = input.Pos;
	////inPos.xyz += time;
	//float pointOne = 100.0;
	//float pointTwo = -100.0;

	///*if (pointOne > inPos.x)
	//{
	//	inPos.x *= 1.0;
	//	inPos.y += sin(time);
	//}
	//if (pointTwo > inPos.z || -pointTwo < inPos.z)
	//{
	//	inPos.z *= 1.0;
	//	inPos.y += sin(time);
	//}*/
	//input.Pos *= Rotation
	//output.Pos = mul(input.Pos, Rotation);
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	return output;
}

float4 WingPixelShader(VS_OUTPUT input) : SV_TARGET
{
	float4 textureColour = modelTexture.Sample(modelSampler, input.Tex);
	return textureColour;
}
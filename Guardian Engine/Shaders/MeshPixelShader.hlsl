#include "PBR/PBRLightShader.hlsli"

cbuffer LightCBuffer : register(b0)
{
	PBRPointLight Lights[50];
	int LightNumber;
	float3 CameraPosition;
};

struct PixelInput
{
	float4 InputPosition : SV_POSITION;
	float3 InputWorldPosition : WORLDPOSITION;
	float2 InputTextureCoord : TEXCOORD;
	float4 InputNormal : NORMAL;
};

Texture2D ObjectTexture : TEXTURE : register(t0);
SamplerState ObjectSampler : SAMPLER : register(s0);

float4 main(PixelInput InputData) : SV_TARGET
{

	return float4(ObjectTexture.Sample(ObjectSampler, InputData.InputTextureCoord).rgb, 1.0f);
}
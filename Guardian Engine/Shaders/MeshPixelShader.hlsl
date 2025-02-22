#include "PBR/PBRLightShader.hlsli"

cbuffer MaterialCBuffer : register(b0)
{
	float3 AlbedoColor;
	float MetallicColor;
	float RoughnessColor;
	float AoColor;
	bool UsingMaps[6];
};

cbuffer LightCBuffer : register(b1)
{
	float3 CameraPosition;
	int PointLightNumber;
	PBRPointLight PointLights[50];
};

struct PixelInput
{
	float4 InputPosition : SV_POSITION;
	float3 InputWorldPosition : WORLDPOSITION;
	float2 InputTextureCoord : TEXCOORD;
	float3 InputNormal : NORMAL;
};

SamplerState ObjectSampler : register(s0);

Texture2D AlbedoTexture : register(t0);
Texture2D MetallicTexture : register(t1);
Texture2D RoughnessTexture : register(t2);
Texture2D NormalTexture : register(t3);
Texture2D AoTexture : register(t4);

float4 main(PixelInput InputData) : SV_TARGET
{
	PBRMaterial MeshMaterial;
	float3 VertexNormal;

	if (UsingMaps[0])
	{
		MeshMaterial.Albedo = pow(AlbedoTexture.Sample(ObjectSampler, InputData.InputTextureCoord).rgb, float3(2.2f, 2.2f, 2.2f));
	}
	else
	{
		MeshMaterial.Albedo = pow(AlbedoColor, float3(2.2f, 2.2f, 2.2f));
	}

	if (UsingMaps[1])
	{
		MeshMaterial.Metallic = MetallicTexture.Sample(ObjectSampler, InputData.InputTextureCoord).r;
	}
	else
	{
		MeshMaterial.Metallic = MetallicColor;
	}

	if (UsingMaps[2])
	{
		MeshMaterial.Roughness = RoughnessTexture.Sample(ObjectSampler, InputData.InputTextureCoord).r;
	}
	else
	{
		MeshMaterial.Roughness = RoughnessColor;
	}

	if (UsingMaps[3])
	{
		VertexNormal = GetNormalFromTexture(InputData.InputWorldPosition, InputData.InputNormal, NormalTexture, ObjectSampler, InputData.InputTextureCoord);
	}
	else
	{
		VertexNormal = InputData.InputNormal;
	}

	MeshMaterial.Ao = AoColor;

	float3 LightColor = CalculatePBRLighting(PointLights, MeshMaterial, PointLightNumber, VertexNormal, InputData.InputWorldPosition, CameraPosition);

	return float4(LightColor, 1.0f);
}

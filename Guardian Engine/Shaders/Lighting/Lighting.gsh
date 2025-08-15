struct LightingMaterial
{
    float3 Albedo;
    float Roughness;
    float Metallic;
    float Ao;
};

struct PointLight
{
    float3 Position;
    float Strength;
    float3 Color;
};

cbuffer MaterialCBuffer : register(b2)
{
    float3 Albedo;
    float Roughness;
    float Metallic;
    float Ao;

    int EnableAlbedoTexture;
    int EnableRoughnessTexture;
    int EnableMetallicTexture;
    int EnableAoTexture;
    int EnableNormalTexture;
};

SamplerState MaterialSampler : register(s0);
Texture2D AlbedoTexture : register(t0);
Texture2D RoughnessTexture : register(t1);
Texture2D MetallicTexture : register(t2);
Texture2D AoTexture : register(t3);
Texture2D NormalTexture : register(t4);

cbuffer LightCBuffer : register(b3)
{
    PointLight PointLightList[50];
    int PointLightCount;
};

LightingMaterial GenerateMaterial(float2 textureCoord)
{
    LightingMaterial Material;

    if (EnableAlbedoTexture)
    {
        Material.Albedo = pow(AlbedoTexture.Sample(MaterialSampler, textureCoord).rgb, float3(0.2f, 0.2f, 0.2f));
    }
    else
    {
        Material.Albedo = Albedo;
    }

    if (EnableRoughnessTexture)
    {
        Material.Roughness = RoughnessTexture.Sample(MaterialSampler, textureCoord).r;
    }
    else
    {
        Material.Roughness;
    }

    if (EnableMetallicTexture)
    {
        Material.Metallic = MetallicTexture.Sample(MaterialSampler, textureCoord).r;
    }
    else
    {
        Material.Metallic = Metallic;
    }

    if (EnableAoTexture)
    {
        Material.Ao = AoTexture.Sample(MaterialSampler, textureCoord).r;
    }
    else
    {
        Material.Ao = Ao;
    }

    return Material;
}

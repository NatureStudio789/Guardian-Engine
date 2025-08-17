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
    float padding;
    int PointLightCount;
};

static const float PI = 3.14159265359f;

float3 GetNormalFromTexture(float3 worldPosition, float3 normal, Texture2D normalMap, SamplerState samplerState, float2 textureCoord)
{
    float3 tangentNormal = normalMap.Sample(samplerState, textureCoord).rgb * 2.0f - 1.0f;

    float3 Q1 = ddx(worldPosition);
    float3 Q2 = ddy(worldPosition);
    float2 st1 = ddx(textureCoord);
    float2 st2 = ddy(textureCoord);

    float3 N = normalize(normal);
    float3 T = normalize(Q1 * st2.y - Q2 * st1.y);
    float3 B = -normalize(cross(N, T));
    float3x3 TBN = float3x3(T, B, N);

    return mul(tangentNormal, TBN);
}

float3 GenerateNormal(float3 worldPosition, float3 normal, float2 textureCoord)
{
    if (!EnableNormalTexture)
    {
        return normal;
    }

    return GetNormalFromTexture(worldPosition, normal, NormalTexture, MaterialSampler, textureCoord);
}

LightingMaterial GenerateMaterial(float2 textureCoord)
{
    LightingMaterial Material;

    if (EnableAlbedoTexture)
    {
        Material.Albedo = pow(AlbedoTexture.Sample(MaterialSampler, textureCoord).rgb, float3(2.2f, 2.2f, 2.2f));
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
        Material.Roughness = Roughness;
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


float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float num = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);

    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

float3 DirectLighting(PointLight light, LightingMaterial material, float3 worldPosition, float3 cameraPosition, float3 normal)
{
    float3 N = normalize(normal);
    float3 V = normalize(cameraPosition - worldPosition);

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, material.Albedo, material.Metallic);

    float3 L = normalize(light.Position - worldPosition);
    float3 H = normalize(V + L);

    float distance = length(light.Position - worldPosition);
    float attenuation = 1.0f / (distance * distance);
    float3 radiance = light.Color * attenuation * light.Strength;

    float NDF = DistributionGGX(N, H, material.Roughness);
    float G = GeometrySmith(N, V, L, material.Roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);

    float3 numerator = NDF * G * F;
    float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.00001f;
    float3 specular = numerator / denominator;

    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0f - material.Metallic;

    float NdotL = max(dot(N, L), 0.0f);

    return (kD * material.Albedo / PI + specular) * radiance * NdotL;
}

float3 Lighting(PointLight lightList[50], const int lightCount, LightingMaterial material, float3 worldPosition, float3 cameraPosition, float3 normal)
{
    float3 Lo = float3(0.0f, 0.0f, 0.0f);

    [unroll]
    for (int i = 0; i < min(lightCount, 50); i++)
    {
        Lo += DirectLighting(lightList[i], material, worldPosition, cameraPosition, normal);
    }

    float3 ambient = float3(0.03f, 0.03f, 0.03f) * material.Albedo * material.Ao;

    float3 Color = ambient + Lo;
    Color = Color / (Color + float3(1.0f, 1.0f, 1.0f));
    Color = pow(Color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    return Color;
}

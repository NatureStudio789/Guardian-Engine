struct PBRMaterial
{
    float3 Albedo;
    float Metallic;
    float Roughness;
    float Ao;
};

struct PBRDirectionLight
{
    float3 LightDirection;
    float3 LightColor;
};

struct PBRPointLight
{
    float3 LightPosition;
    float LightStrength;
    float3 LightColor;
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

    return normalize(mul(TBN, tangentNormal));
}

float DistributionGGX(float3 n, float3 h, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float nDoth = max(dot(n, h), 0.0f);
    float nDoth2 = nDoth * nDoth;
    
    float denom = (nDoth2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    
    return a2 / denom;
}

float GeometrySchlickGGX(float nDotv, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    float denom = nDotv * (1.0f - k) + k;
    
    return nDotv / denom;
}

float GeometrySmith(float3 normal, float3 viewDir, float3 lightDir, float roughness)
{
    float nDotv = max(dot(normal, viewDir), 0.0f);
    float nDotl = max(dot(normal, lightDir), 0.0f);
    float ggx2 = GeometrySchlickGGX(nDotv, roughness);
    float ggx1 = GeometrySchlickGGX(nDotl, roughness);
    
    return ggx1 * ggx2;
}

float3 GetFresnelF0(float3 albedo, float metallic)
{
    float3 f0 = float3(0.04f, 0.04f, 0.04f);
    f0 = lerp(f0, albedo, metallic);
    
    return f0;
}

float3 FresnelSchlick(float cosTheta, float3 f0)
{
    return f0 + (1.0f - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0f);
}

float3 GammaCorrection(float3 color)
{
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    return color;
}

float3 GetAmbient(float3 albedo, float ao)
{
    return float3(0.03f, 0.03f, 0.03f) * albedo * ao;
}

float3 CalculatePBRSimpleLighting(PBRPointLight light, PBRMaterial material, 
    float3 normal, float3 worldPosition, float3 viewPosition)
{
    float3 N = normal;
    float3 V = normalize(viewPosition - worldPosition);
    
    float3 F0 = GetFresnelF0(material.Albedo, material.Metallic);
    
    float3 Lo = float3(0.0f, 0.0f, 0.0f);
    
    float3 L = normalize(light.LightPosition - worldPosition);
    float3 H = normalize(V + L);
    float distance = length(light.LightPosition - worldPosition);
    float attenuation = 1.0f / (distance * distance);
    float3 radiance = light.LightColor * light.LightStrength * attenuation;
    
    float NDF = DistributionGGX(N, H, material.Roughness);
    float G = GeometrySmith(N, V, L, material.Roughness);
    float3 F = FresnelSchlick(clamp(dot(H, V), 0.0f, 1.0f), F0);
        
    float3 numerator = NDF * G * F;
    float denumerator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
    float3 specular = numerator / denumerator;
        
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0f - material.Metallic;
        
    float NDotL = max(dot(N, L), 0.0f);
        
    Lo += (kD * material.Albedo / PI + specular) * radiance * NDotL;

    float3 color = GetAmbient(material.Albedo, material.Ao) + Lo;
    
    return color;
}

float3 CalculateDirectionLight(PBRDirectionLight light, PBRMaterial material, 
    float3 normal, float3 worldPosition, float3 viewPosition)
{
    float3 N = normal;
    float3 V = normalize(viewPosition - worldPosition);
    float3 L = normalize(-light.LightDirection);
    float3 H = normalize(V + L);

    float3 F0 = GetFresnelF0(material.Albedo, material.Metallic);

    float NDF = DistributionGGX(N, H, material.Roughness);
    float G = GeometrySmith(N, V, L, material.Roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);

    float3 nominator = NDF * G * F;
    float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
    float3 specular = nominator / denominator;

    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0f - material.Metallic;

    float NdotL = max(dot(N, L), 0.0f);

    float Lo = (kD / PI + specular) * light.LightColor * NdotL;

    float3 color = Lo;

    return color;
}

float3 CalculatePBRLighting(PBRPointLight lights[50], PBRMaterial material, const int lightNumber,
    float3 normal, float3 pixelInWorldPosition, float3 viewPosition)
{
    float3 N = normalize(normal);
    float3 V = normalize(viewPosition - pixelInWorldPosition);
    
    float3 F0 = GetFresnelF0(material.Albedo, material.Metallic);
    
    float3 Lo = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < min(lightNumber, 50); i++)
    {
        float3 L = normalize(lights[i].LightPosition - pixelInWorldPosition);
        float3 H = normalize(V + L);
        float distance = length(lights[i].LightPosition - pixelInWorldPosition);
        float attenuation = 1.0f / (distance * distance);
        float3 radiance = lights[i].LightColor * lights[i].LightStrength * attenuation;
        
        float NDF = DistributionGGX(N, H, material.Roughness);
        float G = GeometrySmith(N, V, L, material.Roughness);
        float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);
        
        float3 numerator = NDF * G * F;
        float denumerator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
        float3 specular = numerator / denumerator;
        
        float3 kS = F;
        float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        kD *= 1.0f - material.Metallic;
        
        float NDotL = max(dot(N, L), 0.0f);
        
        Lo += (kD * material.Albedo / PI + specular) * radiance * NDotL;
    }
    
    float3 color = GetAmbient(material.Albedo, material.Ao) + Lo;
    
    return color;
}

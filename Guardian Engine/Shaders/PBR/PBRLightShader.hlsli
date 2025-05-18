struct Material
{
    float3 Albedo;
    float Metallic;
    float Roughness;
    float Ao;
};

struct DirectionalLight
{
    float3 LightDirection;
    float LightStrength;
    float3 LightColor;
};

struct PointLight
{
    float3 LightPosition;
    float LightStrength;
    float3 LightColor;
    float4x4 LightMatrix;
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

    return mul(TBN, tangentNormal);
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

float CalculateShadow(float4 positionLightSpace, Texture2D shadowMap, SamplerState samplerState)
{
    float3 projectionCoords = positionLightSpace.xyz / positionLightSpace.w;
    projectionCoords = projectionCoords * 0.5f + 0.5f;
    
    float closestDepth = shadowMap.Sample(samplerState, projectionCoords.xy).r;
    float currentDepth = projectionCoords.z - 0.005f;
    float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;
    
    return shadow;
}

float3 CalculateLight(float3 L, float3 radiance, float3 N, float3 V, 
    float3 albedo, float metallic, float roughness, float4 positionLightSpace, Texture2D shadowMap, SamplerState samplerState)
{
    float3 H = normalize(V + L);
    
    float3 F0 = GetFresnelF0(albedo, roughness);
    
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);

    float3 numerator = NDF * G * F;
    float denomiator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
    float3 specular = numerator / denomiator;
    
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0f - metallic;
    float3 diffuse = kD * albedo / PI;
    
    float NdotL = max(dot(N, L), 0.0f);
    return (diffuse + specular) * radiance * NdotL * (1.0f - CalculateShadow(positionLightSpace, shadowMap, samplerState));
}

/*float3 CalculatePBRLighting(PointLight lights[50], Material material, const int lightNumber,
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
}*/

float3 CalculatePointLight(PointLight light, Material material, 
    float3 normal, float3 worldPosition, float3 viewPosition, float4 positionLightSpace, Texture2D shadowMap, SamplerState samplerState)
{
    float3 LVector = light.LightPosition - worldPosition;
    float distance = length(LVector);
    float3 L = LVector / distance;
    
    float attenuation = 1.0f / (distance * distance);
    float3 radiance = light.LightColor * light.LightStrength * attenuation;
    
    return CalculateLight(L, radiance, normalize(normal), normalize(viewPosition - worldPosition),
            material.Albedo, material.Metallic, material.Roughness, positionLightSpace, shadowMap, samplerState);
}

float3 CalculateMultiplePointLights(PointLight lights[50], Material material, const int lightNumber,
    float3 normal, float3 pixelInWorldPosition, float3 viewPosition, float4 positionLightSpace, Texture2D shadowMap, SamplerState samplerState)
{
    float3 LightColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < min(lightNumber, 50); i++)
    {
        LightColor += CalculatePointLight(lights[i], material, normal, pixelInWorldPosition, viewPosition, positionLightSpace, shadowMap, samplerState);
    }
    
    return LightColor;
}

float3 CalculateDirectionalLight(DirectionalLight light, Material material, 
    float3 normal, float3 worldPosition, float3 viewPosition, float4 positionLightSpace, Texture2D shadowMap, SamplerState samplerState)
{
    float3 L = normalize(-light.LightDirection);
    float3 radiance = light.LightColor * light.LightStrength;
    
    return CalculateLight(L, radiance, normalize(normal), normalize(viewPosition - worldPosition),
        material.Albedo, material.Metallic, material.Roughness, positionLightSpace, shadowMap, samplerState);
}


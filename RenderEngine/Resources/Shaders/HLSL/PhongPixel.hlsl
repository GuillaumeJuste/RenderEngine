#ifndef RENDERENGINE_SHADER_PHONGFRAGMENT
#define RENDERENGINE_SHADER_PHONGFRAGMENT

#include "Light/PointLight.hlsl"
#include "Light/DirectionalLight.hlsl"
#include "Light/SpotLight.hlsl"
#include "Common/Normal.hlsl"

struct PSInput
{
    float4 svPosition : SV_POSITION;
    float3 fragPos : POSITION0;
    float3 normal : NORMAL;
    float3 texCoord : TEXCOORD;
    float3 cameraPos : POSITION1;
    float3 tangent : TANGENT;
};

[[vk::combinedImageSampler]]
Texture2D<float4> albedoText : register(t0, space1);
[[vk::combinedImageSampler]]
SamplerState albedoSampler : register(s0, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> metalnessMap : register(t1, space1);
[[vk::combinedImageSampler]]
SamplerState metalnessMapSampler : register(s1, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> normalMap : register(t2, space1);
[[vk::combinedImageSampler]]
SamplerState normalMapSampler : register(s2, space1);

cbuffer MaterialBufferObject : register(b3, space1)
{
    PhongMaterial material;
}

cbuffer pointLightsBuffer : register(b4, space1)
{
    PointLight pointLightsBuffer[LIGHT_COUNT];
}

cbuffer directionalLightsBuffer : register(b5, space1)
{
    DirectionalLight directionalLightsBuffer[LIGHT_COUNT];
}

cbuffer spotLightsBuffer : register(b6, space1)
{
    SpotLight spotLightsBuffer[LIGHT_COUNT];
}

float4 main(PSInput _input) : SV_TARGET
{
    float3 albedo = albedoText.Sample(albedoSampler, _input.texCoord.xy).xyz;
    float metalness = metalnessMap.Sample(metalnessMapSampler, _input.texCoord.xy).x;

    float3 normal = getNormalFromMap(_input.texCoord.xy, _input.normal, _input.tangent, normalMap, normalMapSampler);
    float3 viewDirection = normalize(_input.cameraPos - _input.fragPos);

    float3 color = float3(0.0, 0.0, 0.0);
    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        PointLight element = pointLightsBuffer[i];
        if (element.enable == true)
        {
            Light light = GenerateLightFromPointLight(_input.fragPos, element);
            color += ComputePhongLighting(light, material, normal, viewDirection, albedo, metalness);
        }
    }

    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        DirectionalLight element = directionalLightsBuffer[i];
        if (element.enable == true)
        {
            Light light = GenerateLightFromDirectionalLight(element);
            color += ComputePhongLighting(light, material, normal, viewDirection, albedo, metalness);
        }
    }

    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        SpotLight element = spotLightsBuffer[i];
        if (element.enable == true)
        {
            Light light = GenerateLightFromSpotLight(_input.fragPos, element);
            if (light.isValid)
                color += ComputePhongLighting(light, material, normal, viewDirection, albedo, metalness);
        }
    }

    return float4(color, 1.0);
}

#endif
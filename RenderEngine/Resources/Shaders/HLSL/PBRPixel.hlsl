#ifndef RENDERENGINE_SHADER_PBRFRAGMENT
#define RENDERENGINE_SHADER_PBRFRAGMENT

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
Texture2D<float4> roughnessMap : register(t2, space1);
[[vk::combinedImageSampler]]
SamplerState roughnessMapSampler : register(s2, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> normalMap : register(t3, space1);
[[vk::combinedImageSampler]]
SamplerState normalMapSampler : register(s3, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> aoMap : register(t4, space1);
[[vk::combinedImageSampler]]
SamplerState aoMapSampler : register(s4, space1);

cbuffer pointLightsBuffer: register(b5, space1)
{
    PointLight pointLightsBuffer[LIGHT_COUNT];
}

cbuffer directionalLightsBuffer : register(b6, space1)
{
    DirectionalLight directionalLightsBuffer[LIGHT_COUNT];
}

cbuffer spotLightsBuffer : register(b7, space1)
{
    SpotLight spotLightsBuffer[LIGHT_COUNT];
}

[[vk::combinedImageSampler]]
TextureCube irradianceMap : register(t8, space1);
[[vk::combinedImageSampler]]
SamplerState irradianceSampler : register(s8, space1);

[[vk::combinedImageSampler]]
TextureCube prefilteredMap : register(t9, space1);
[[vk::combinedImageSampler]]
SamplerState prefilteredSampler : register(s9, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> BRDFlut : register(t10, space1);
[[vk::combinedImageSampler]]
SamplerState BRDFlutSampler : register(s10, space1);


float3 prefilteredReflection(float3 _reflection, float _roughness);

float4 main(PSInput _input) : SV_TARGET
{
    float3 albedo = albedoText.Sample(albedoSampler, _input.texCoord.xy).xyz;
    float metalness = metalnessMap.Sample(metalnessMapSampler, _input.texCoord.xy).x;
    float roughness = roughnessMap.Sample(roughnessMapSampler, _input.texCoord.xy).x;
    float ao = aoMap.Sample(aoMapSampler, _input.texCoord.xy).x;

    float3 normal = getNormalFromMap(_input.texCoord.xy, _input.normal, _input.tangent, normalMap, normalMapSampler);
    float3 viewDirection = normalize(_input.cameraPos - _input.fragPos);
    float3 reflection = reflect(-viewDirection, normal);

	// calculate reflectance at normal incidence;
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 =  lerp(F0, albedo, metalness);

    float3 Lo = float3(0.0, 0.0, 0.0);

    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        PointLight element = pointLightsBuffer[i];
        if (element.enable == true)
        {
            Light light = GenerateLightFromPointLight(_input.fragPos, element);
            Lo += ComputePBRLighting(light, normal, viewDirection, albedo, metalness, roughness, F0);
        }
    }

    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        DirectionalLight element = directionalLightsBuffer[i];
        if (element.enable == true)
        {
            Light light = GenerateLightFromDirectionalLight(element);
            Lo += ComputePBRLighting(light, normal, viewDirection, albedo, metalness, roughness, F0);
        }
    }
    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        SpotLight element = spotLightsBuffer[i];
        if (element.enable == true)
        {
            Light light = GenerateLightFromSpotLight(_input.fragPos, element);
            if (light.isValid)
                Lo += ComputePBRLighting(light, normal, viewDirection, albedo, metalness, roughness, F0);
        }
    }
    float3 kS = FresnelSchlickRoughness(max(dot(normal, viewDirection), 0.0), F0, roughness);
    float3 kD = 1.0 - kS;
    kD *= 1.0 - metalness;
    float3 irradiance = irradianceMap.Sample(irradianceSampler, normal).rgb;
    float3 diffuse = irradiance * albedo;

    float3 prefilteredColor = prefilteredReflection(reflection, roughness);
    float2 brdf = BRDFlut.Sample(BRDFlutSampler, float2(max(dot(normal, viewDirection), 0.0), roughness)).rg;
    float3 specular = prefilteredColor * (kS * brdf.x + brdf.y);

    float3 ambient = (kD * diffuse + specular) * ao;

    float3 color = ambient + Lo;

    color = color / (color + float3(1.0, 1.0, 1.0));

    return float4(color, 1.0);
}

float3 prefilteredReflection(float3 _reflection, float _roughness)
{
    const float MAX_REFLECTION_LOD = 10.0; // todo: param/const
    float lod = _roughness * MAX_REFLECTION_LOD;
    float lodf = floor(lod);
    float lodc = ceil(lod);
    float3 a = prefilteredMap.SampleLevel(prefilteredSampler, _reflection, lodf).rgb;
    float3 b = prefilteredMap.SampleLevel(prefilteredSampler, _reflection, lodc).rgb;
    return lerp(a, b, lod - lodf);
}

#endif
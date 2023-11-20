#ifndef RENDERENGINE_SHADER_TEXTURETOCUBEMAP
#define RENDERENGINE_SHADER_TEXTURETOCUBEMAP

struct PSInput
{
    float4 svPosition : SV_POSITION;
    float3 fragPos : FRAGPOS;
};

[[vk::combinedImageSampler]]
Texture2D<float4> equirectangularMap : register(t0, space0);
[[vk::combinedImageSampler]]
SamplerState equirectangularMapSampler : register(s0, space0);

static const float2 invAtan = float2(0.1591, 0.3183);

float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(PSInput _input) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(_input.fragPos));
    uv.y *= -1;
    
    return equirectangularMap.Sample(equirectangularMapSampler, uv);
}

#endif
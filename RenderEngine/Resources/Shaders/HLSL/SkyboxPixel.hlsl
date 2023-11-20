#ifndef RENDERENGINE_SHADER_SKYBOXPIXEL
#define RENDERENGINE_SHADER_SKYBOXPIXEL

struct PSInput
{
    float4 svPosition : SV_POSITION;
    float3 fragPos : FRAGPOS;
};

[[vk::combinedImageSampler]]
TextureCube cubemap : register(t0, space1);
[[vk::combinedImageSampler]]
SamplerState cubemapSampler : register(s0, space1);

float4 main(PSInput _input) : SV_TARGET
{
    return cubemap.Sample(cubemapSampler, _input.fragPos);
}

#endif
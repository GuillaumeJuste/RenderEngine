#ifndef RENDERENGINE_SHADER_TEXTUREPIXEL
#define RENDERENGINE_SHADER_TEXTUREPIXEL

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

float4 main(PSInput _input) : SV_TARGET
{
    return albedoText.Sample(albedoSampler, _input.texCoord.xy);
}

#endif
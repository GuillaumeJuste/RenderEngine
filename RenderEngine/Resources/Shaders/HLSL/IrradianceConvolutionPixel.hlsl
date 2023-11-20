#ifndef RENDERENGINE_SHADER_IRRADIANCECONVOLUTION
#define RENDERENGINE_SHADER_IRRADIANCECONVOLUTION

#include "Common/Pi.hlsl"

struct PSInput
{
    float4 svPosition : SV_POSITION;
    float3 fragPos : FRAGPOS;
};

[[vk::combinedImageSampler]]
TextureCube environmentMap : register(t0, space0);
[[vk::combinedImageSampler]]
SamplerState environmentMapSampler : register(s0, space0);

float4 main(PSInput _input) : SV_TARGET
{
    // The world vector acts as the normal of a tangent surface
    // from the origin, aligned to WorldPos. Given this normal, calculate all
    // incoming radiance of the environment. The result of this radiance
    // is the radiance of light coming from -Normal direction, which is what
    // we use in the PBR shader to sample irradiance.
    float3 N = normalize(_input.fragPos);

    float3 irradiance = float3(0.0, 0.0, 0.0);
    
    // tangent space calculation from origin point
    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));
       
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += environmentMap.Sample(environmentMapSampler, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    return float4(irradiance, 1.0);
}

#endif
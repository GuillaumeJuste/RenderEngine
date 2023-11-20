#ifndef RENDERENGINE_SHADER_SKYBOXVERTEX
#define RENDERENGINE_SHADER_SKYBOXVERTEX

#include "Common/CameraBuffer.hlsl"

cbuffer cbo : register(b0, space0)
{
    CBO cbo;
}

struct vertexInfo
{
    [[vk::location(0)]]
    float3 position : POSITION;
    [[vk::location(1)]]
    float3 normal : NORMAL;
    [[vk::location(2)]]
    float3 color : COLOR;
    [[vk::location(3)]]
    float3 tangent : TANGENT;
};

struct VSOutput
{
    float4 svPosition : SV_POSITION;
    float3 fragPos : FRAGPOS;
};

VSOutput main(vertexInfo input, uint VertexIndex : SV_VertexID)
{
    VSOutput output;

    output.fragPos = input.position;
    
    float4x4 staticView = float4x4(float4(cbo.invView[0].xyz, 0),
    float4(cbo.invView[1].xyz, 0),
    float4(cbo.invView[2].xyz, 0),
    float4(0, 0, 0, 1)); //Remove camera translation.
    float4x4 VP = mul(cbo.proj, staticView);
    output.svPosition = mul(VP, float4(input.position, 1.0));
    output.svPosition = output.svPosition.xyww;
    
    return output;
}

#endif
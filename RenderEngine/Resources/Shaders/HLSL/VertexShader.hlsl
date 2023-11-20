#ifndef RENDERENGINE_SHADER_VERTEXSHADER
#define RENDERENGINE_SHADER_VERTEXSHADER

#include "Common/CameraBuffer.hlsl"

struct UBO
{
    float4x4 model;
};

cbuffer ubo : register(b0, space0) { UBO ubo; }

cbuffer cbo : register(b1, space0) { CBO cbo; }

struct vertexInfo
{
    [[vk::location(0)]]
    float3 position : POSITION;
    [[vk::location(1)]]
    float3 normal: NORMAL;
    [[vk::location(2)]]
    float3 color : COLOR;
    [[vk::location(3)]]
    float3 tangent : TANGENT;
};

struct VSOutput
{
    float4 svPosition : SV_POSITION;
    float3 fragPos : FRAGPOS;
    float3 normal : NORMAL;
    float3 texCoord : TEXCOORD;
    float3 cameraPos : CAMERAPOS;
    float3 tangent : TANGENT;
};

VSOutput main(vertexInfo input, uint VertexIndex : SV_VertexID)
{
    VSOutput output;

    float4x4 mvp = mul(cbo.proj, cbo.invView);
    mvp = mul(mvp, ubo.model);
    
    output.svPosition = mul(mvp, float4(input.position, 1.0));

    output.texCoord = input.color;
    output.fragPos = (float3) mul(ubo.model, float4(input.position, 1.0));

    float3x3 modelMat3 = (float3x3) ubo.model;
    output.normal = mul(modelMat3 , input.normal);
    output.tangent = mul(modelMat3, input.tangent);
    output.cameraPos = cbo.cameraPos;
    
    return output;
}

#endif
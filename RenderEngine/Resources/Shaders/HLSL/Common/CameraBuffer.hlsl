#ifndef RENDERENGINE_SHADER_CAMERABUFFER
#define RENDERENGINE_SHADER_CAMERABUFFER

struct CBO
{
    float4x4 invView;
    float4x4 proj;
    float3 cameraPos;
};

#endif
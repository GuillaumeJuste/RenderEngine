#ifndef RENDERENGINE_SHADER_MATERIAL
#define RENDERENGINE_SHADER_MATERIAL

struct PhongMaterial
{
    float shininess;
    float3 Ka;
    float3 Kd;
    float3 Ks;
};

#endif
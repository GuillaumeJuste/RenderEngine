#ifndef RENDERENGINE_SHADER_POINTLIGHT
#define RENDERENGINE_SHADER_POINTLIGHT

#include "Light.hlsl"

struct PointLight
{
    float3 position;
    float3 color;
    float intensity;
    float range;
    bool enable;
};

Light GenerateLightFromPointLight(float3 _fragPos, PointLight _light)
{
    Light light;
    light.direction = normalize(_light.position - _fragPos);

    float distance = length(_light.position - _fragPos);
    float attenuation = ComputeAttenuation(distance, _light.range);
    light.radiance = _light.color * _light.intensity * attenuation;
    light.isValid = true;
    
    return light;
}

#endif
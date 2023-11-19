#ifndef RENDERENGINE_SHADER_DIRECTIONALLIGHT
#define RENDERENGINE_SHADER_DIRECTIONALLIGHT

#include "Light.hlsl"

struct DirectionalLight
{
    float3 color;
    float3 direction;
    float intensity;
    bool enable;
};

Light GenerateLightFromDirectionalLight(DirectionalLight _light)
{
    Light light;
    light.direction = normalize(-_light.direction);
    light.radiance = _light.color * _light.intensity;
    light.isValid = true;
    
    return light;
}

#endif
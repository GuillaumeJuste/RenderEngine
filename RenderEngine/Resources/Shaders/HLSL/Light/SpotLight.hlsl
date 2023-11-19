#ifndef RENDERENGINE_SHADER_SPOTLIGHT
#define RENDERENGINE_SHADER_SPOTLIGHT

#include "Light.hlsl"

struct SpotLight
{
    float3 position;
    float3 color;
    float3 direction;
    float intensity;
    float range;
    float cutOff;
    bool enable;
};

Light GenerateLightFromSpotLight(float3 _fragPos, SpotLight _light)
{
    Light light;
    light.direction = normalize(_light.position - _fragPos);
    float lightTheta = dot(light.direction, normalize(-_light.direction));
	
    if (lightTheta > _light.cutOff)
    {
        float distance = length(_light.position - _fragPos);
        float attenuation = ComputeAttenuation(distance, _light.range);
        light.radiance = _light.color * _light.intensity * attenuation;
        light.isValid = true;
        
    }
    else
    {
        light.isValid = false;
    }
    return light;
}

#endif
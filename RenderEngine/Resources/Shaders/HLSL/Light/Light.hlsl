#ifndef RENDERENGINE_SHADER_LIGHT
#define RENDERENGINE_SHADER_LIGHT

#include "../Common/Material.hlsl"
#include "../Common/Pi.hlsl"

struct Light
{
    float3 direction;
    float3 radiance;
    bool isValid;
};

// _N : normal
// _H : halfwayDirection
// _roughness : material roughness from map
float DistributionGGX(float3 _N, float3 _H, float _roughness)
{
    float a = _roughness * _roughness;
    float a2 = a * a;
    float NdotH = max(dot(_N, _H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// _H : halfwayDirection
// _V : viewDirection
// _F0 : reflectance at normal incidence
float3 FresnelSchlickRoughness(float _cosTheta, float3 _F0, float _roughness)
{
    return _F0 + (max(float3(1.0 - _roughness, 1.0 - _roughness, 1.0 - _roughness), _F0) - _F0) * pow(clamp(1.0 - _cosTheta, 0.0, 1.0), 5.0);
}

// _NdotV : normal and  view dot product
// _roughness : material roughness
float GeometrySchlickGGX(float _NdotV, float _roughness)
{
    float nom = _NdotV;
    float denom = _NdotV * (1.0 - _roughness) + _roughness;
	
    return nom / denom;
}

// _N : normal
// _V : viewDirection
// _L : lightDirection
// _roughness : material roughness
float GeometrySmith(float3 _N, float3 _V, float3 _L, float _roughness)
{
    float NdotV = max(dot(_N, _V), 0.0);
    float NdotL = max(dot(_N, _L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, _roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, _roughness);
	
    return ggx1 * ggx2;
}

float3 ComputePBRLighting(Light _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0)
{
    float3 halfwayDirection = normalize(_viewDirection + _light.direction);

    float D = DistributionGGX(_normal, halfwayDirection, _roughness);
    float3 F = FresnelSchlickRoughness(max(dot(halfwayDirection, _viewDirection), 0.0), _F0, _roughness);
    float G = GeometrySmith(_normal, _viewDirection, _light.direction, _roughness);

    float3 numerator = D * F * G;
    float denominator = 4.0 * max(dot(_normal, _viewDirection), 0.0) * max(dot(_normal, _light.direction), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    float3 specular = numerator / denominator;

    float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - _metalness;

	// scale light by NdotL
    float NdotL = max(dot(_normal, _light.direction), 0.0);

	// add to outgoing radiance Lo
    return (kD * _albdeo / PI + specular) * _light.radiance * NdotL;
}

float ComputeAttenuation(float _distance, float _lightRange)
{
    float linearFactor = 4.5 / _lightRange;
    float quadraticFactor = 75.0 / (_lightRange * _lightRange);
    return 1.0 / (1.0 + linearFactor * _distance + quadraticFactor * (_distance * _distance));
}

float3 ComputePhongLighting(Light _light, PhongMaterial _material, float3 _normal, float3 _viewDirection, float3 _albedo, float _metalness)
{
    float3 ambient = _material.Ka * _albedo;
    float3 diffuse = float3(0.0, 0.0, 0.0);
    float3 specular = float3(0.0, 0.0, 0.0);

    float cosTheta = dot(_normal, _light.direction);

	// Only if the light is visible from the surface point.
    if (cosTheta > 0.0)
    {
        diffuse = _material.Kd * _albedo * cosTheta;

        float3 reflectDirection = 2.0 * dot(_light.direction, _normal) * _normal - _light.direction;

        specular = _material.Ks * _albedo * pow(max(dot(_viewDirection, reflectDirection), 0.0), _material.shininess) * _metalness;
    }

    return (ambient + diffuse + specular) * _light.radiance;
}

float3 ComputeBlinnPhongLighting(Light _light, PhongMaterial _material, float3 _normal, float3 _viewDirection, float3 _albedo, float _metalness)
{
    float3 ambient = _material.Ka * _albedo;
    float3 diffuse = float3(0.0, 0.0, 0.0);
    float3 specular = float3(0.0, 0.0, 0.0);

    float cosTheta = dot(_normal, _light.direction);

	// Only if the light is visible from the surface point.
    if (cosTheta > 0.0)
    {
        diffuse = _material.Kd * _albedo * cosTheta;

        float3 halfwayDirection = normalize(_light.direction + _viewDirection);

        specular = _material.Ks * _albedo * pow(max(dot(_normal, halfwayDirection), 0.0), _material.shininess) * _metalness;
    }

    return (ambient + diffuse + specular) * _light.radiance;
}

#endif
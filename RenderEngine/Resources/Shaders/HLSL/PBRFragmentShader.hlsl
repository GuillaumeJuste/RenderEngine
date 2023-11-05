struct VSOutput
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

[[vk::combinedImageSampler]]
Texture2D<float4> metalnessMap : register(t1, space1);
[[vk::combinedImageSampler]]
SamplerState metalnessMapSampler : register(s1, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> roughnessMap : register(t2, space1);
[[vk::combinedImageSampler]]
SamplerState roughnessMapSampler : register(s2, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> normalMap : register(t3, space1);
[[vk::combinedImageSampler]]
SamplerState normalMapSampler : register(s3, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> aoMap : register(t4, space1);
[[vk::combinedImageSampler]]
SamplerState aoMapSampler : register(s4, space1);

struct PointLight
{
    bool enable;
    float3 position;
    float3 color;
    float intensity;
    float range;
};

cbuffer pointLightsBuffer : register(b5, space1)
{
    int pointLightCount;
    StructuredBuffer<PointLight> pointLightsBuffers[];
}

struct DirectionalLight
{
    bool enable;
    float3 color;
    float3 direction;
    float intensity;
};

cbuffer directionalLightsBuffer : register(b6, space1)
{
    int directionalLightCount;
    StructuredBuffer<DirectionalLight> directionalLightsBuffer[];
}

struct SpotLight
{
    bool enable;
    float3 position;
    float3 color;
    float3 direction;
    float intensity;
    float range;
    float cutOff;
	
};

cbuffer spotLightsBuffer : register(b7, space1)
{
    int spotlightCount;
    StructuredBuffer<SpotLight> spotLightsBuffer[];
}

[[vk::combinedImageSampler]]
TextureCube irradianceMap : register(t8, space1);
[[vk::combinedImageSampler]]
SamplerState irradianceSampler : register(s8, space1);

[[vk::combinedImageSampler]]
TextureCube prefilteredMap : register(t9, space1);
[[vk::combinedImageSampler]]
SamplerState prefilteredSampler : register(s9, space1);

[[vk::combinedImageSampler]]
Texture2D<float4> BRDFlut : register(t10, space1);
[[vk::combinedImageSampler]]
SamplerState BRDFlutSampler : register(s10, space1);

static const float PI = 3.14159265359;

struct Light
{
    float3 direction;
    float3 radiance;
};


// _N : normal
// _H : halfwayDirection
// _roughness : material roughness from map
float DistributionGGX(float3 _N, float3 _H, float _roughness);

// _H : halfwayDirection
// _V : viewDirection
// _F0 : reflectance at normal incidence
float3 FresnelSchlickRoughness(float _cosTheta, float3 _F0, float _roughness);

// _NdotV : normal and  view dot product
// _roughness : material roughness
float GeometrySchlickGGX(float _NdotV, float _roughness);

// _N : normale
// _V : viewDirection
// _L : lightDirection
// _roughness : material roughness
float GeometrySmith(float3 _N, float3 _V, float3 _L, float _roughness);

float3 prefilteredReflection(float3 _reflection, float _roughness);

float3 getNormalFromMap(VSOutput _input);

float ComputeAttenuation(float _distance, float _lightRange);
float3 ComputePointLightLighting(float3 _fragPos, PointLight _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0);
float3 ComputeDirectionalLightLighting(DirectionalLight _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0);
float3 ComputeSpotLightLighting(float3 _fragPos, SpotLight _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0);

float3 ComputeLighting(Light _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0);

float4 main(VSOutput _input) : SV_TARGET
{
    float3 albedo = albedoText.Sample(albedoSampler, _input.texCoord.xy).xyz;
    float metalness = metalnessMap.Sample(metalnessMapSampler, _input.texCoord.xy).x;
    float roughness = roughnessMap.Sample(roughnessMapSampler, _input.texCoord.xy).x;
    float ao = aoMap.Sample(aoMapSampler, _input.texCoord.xy).x;

    float3 normal = getNormalFromMap(_input);
    float3 viewDirection = normalize(_input.cameraPos - _input.fragPos);
    float3 reflection = reflect(-viewDirection, normal);

	// calculate reflectance at normal incidence;
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 =  lerp(F0, albedo, metalness);

    float3 Lo = float3(0.0, 0.0, 0.0);

    //for (int i = 0; i < pointLightCount; ++i)
    //{
    //    PointLight element = pointLightsBuffers[i];
    //    if (element.enable == true)
    //    {
    //        Lo += ComputePointLightLighting(_input.fragPos, element, normal, viewDirection, albedo, metalness, roughness, F0);
    //    }
    //}

    //for (int i = 0; i < directionalLightCount; i++)
    //{
    //    DirectionalLight element = directionalLightsBuffer[i];
    //    if (element.enable == true)
    //        Lo += ComputeDirectionalLightLighting(element, normal, viewDirection, albedo, metalness, roughness, F0);
    //}

    //for (int i = 0; i < spotlightCount; i++)
    //{
    //    SpotLight element = spotLightsBuffer[i];
    //    if (element.enable == true)
    //        Lo += ComputeSpotLightLighting(_input.fragPos, element, normal, viewDirection, albedo, metalness, roughness, F0);
    //}

    float3 kS = FresnelSchlickRoughness(max(dot(normal, viewDirection), 0.0), F0, roughness);
    float3 kD = 1.0 - kS;
    kD *= 1.0 - metalness;
    float3 irradiance = irradianceMap.Sample(irradianceSampler, normal).rgb;
    float3 diffuse = irradiance * albedo;

    float3 prefilteredColor = prefilteredReflection(reflection, roughness);
    float2 brdf = BRDFlut.Sample(BRDFlutSampler, float2(max(dot(normal, viewDirection), 0.0), roughness)).rg;
    float3 specular = prefilteredColor * (kS * brdf.x + brdf.y);

    float3 ambient = (kD * diffuse + specular) * ao;

    float3 color = ambient + Lo;

    color = color / (color + float3(1.0, 1.0, 1.0));

    return float4(color, 1.0);
}

float3 ComputePointLightLighting(float3 _fragPos, PointLight _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0)
{
    Light light;
    light.direction = normalize(_light.position - _fragPos);

    float distance = length(_light.position - _fragPos);
    float attenuation = ComputeAttenuation(distance, _light.range);
    light.radiance = _light.color * _light.intensity * attenuation;

    return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
}

float3 ComputeDirectionalLightLighting(DirectionalLight _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0)
{
    Light light;
    light.direction = normalize(-_light.direction);
    light.radiance = _light.color * _light.intensity;

    return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
}

float3 ComputeSpotLightLighting(float3 _fragPos, SpotLight _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0)
{
    Light light;
    light.direction = normalize(_light.position - _fragPos);
    float lightTheta = dot(light.direction, normalize(-_light.direction));
	
    if (lightTheta > _light.cutOff)
    {
        float distance = length(_light.position - _fragPos);
        float attenuation = ComputeAttenuation(distance, _light.range);
        light.radiance = _light.color * _light.intensity * attenuation;

        return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
    }
    return float3(0.0, 0.0, 0.0);
}

float3 ComputeLighting(Light _light, float3 _normal, float3 _viewDirection, float3 _albdeo, float _metalness, float _roughness, float3 _F0)
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

float3 FresnelSchlickRoughness(float _cosTheta, float3 _F0, float _roughness)
{
    return _F0 + (max(float3(1.0 - _roughness, 1.0 - _roughness, 1.0 - _roughness), _F0) - _F0) * pow(clamp(1.0 - _cosTheta, 0.0, 1.0), 5.0);
}

float GeometrySchlickGGX(float _NdotV, float _roughness)
{
    float nom = _NdotV;
    float denom = _NdotV * (1.0 - _roughness) + _roughness;
	
    return nom / denom;
}
  
float GeometrySmith(float3 _N, float3 _V, float3 _L, float _roughness)
{
    float NdotV = max(dot(_N, _V), 0.0);
    float NdotL = max(dot(_N, _L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, _roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, _roughness);
	
    return ggx1 * ggx2;
}

float ComputeAttenuation(float _distance, float _lightRange)
{
    float linearFactor  = 4.5 / _lightRange;
    float quadraticFactor = 75.0 / (_lightRange * _lightRange);
    return 1.0 / (1.0 + linearFactor * _distance + quadraticFactor * (_distance * _distance));
}

float3 prefilteredReflection(float3 _reflection, float _roughness)
{
    const float MAX_REFLECTION_LOD = 10.0; // todo: param/const
    float lod = _roughness * MAX_REFLECTION_LOD;
    float lodf = floor(lod);
    float lodc = ceil(lod);
    float3 a = prefilteredMap.SampleLevel(prefilteredSampler, _reflection, lodf).rgb;
    float3 b = prefilteredMap.SampleLevel(prefilteredSampler, _reflection, lodc).rgb;
    return lerp(a, b, lod - lodf);
}

float3 getNormalFromMap(VSOutput _input)
{
    float3 N = normalize(_input.normal);
    float3 T = normalize(_input.tangent);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T.x, B.x, N.x,
                            T.y, B.y, N.y,
                            T.z, B.z, N.z);
    
    return normalize(mul(TBN, (normalMap.Sample(normalMapSampler, _input.texCoord.xy).rgb * 2.0 - 1.0)));
}
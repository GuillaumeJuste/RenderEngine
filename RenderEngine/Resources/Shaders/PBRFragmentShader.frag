#version 450
#pragma shader_stage(fragment)

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 interpNormal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} fsIn;

layout(set = 1, binding = 0) uniform sampler2D albedoSampler;
layout(set = 1, binding = 1) uniform sampler2D metalnessMapSampler;
layout(set = 1, binding = 6) uniform sampler2D roughnessMapSampler;
layout(set = 1, binding = 7) uniform sampler2D aoMapSampler;

struct PointLight
{
	bool enable;
	vec3 position;
	vec3 color;
	float intensity;
	float range;
};

layout (set = 1,binding = 3) buffer PointLightData
{
	PointLight lights[];
} pointLightsBuffer;

struct DirectionalLight
{
	bool enable;
	vec3 color;
	vec3 direction;
	float intensity;
};

layout (set = 1,binding = 4) buffer DirectionalLightData
{
	DirectionalLight lights[];
} directionalLightsBuffer;

struct SpotLight
{
	bool enable;
	vec3 position;
	vec3 color;
	vec3 direction;
	float intensity;
	float range;
	float cutOff;
	
};

layout (set = 1,binding = 5) buffer SpotLightData
{
	SpotLight lights[];
} spotLightsBuffer;

layout(location = 0) out vec4 outColor;

float roughness = 0.2;

const float PI = 3.14159265359;

struct Light
{
	vec3 color;
	vec3 direction;
	vec3 radiance;
};


// _N : normal
// _H : halfwayDirection
// _roughness : material roughness from map
float DistributionGGX(vec3 _N, vec3 _H, float _roughness);

// _H : halfwayDirection
// _V : viewDirection
// _F0 : reflectance at normal incidence
vec3 FresnelSchlick(vec3 _H, vec3 _V, vec3 _F0);

// _NdotV : normal and  view dot product
// _roughness : material roughness
float GeometrySchlickGGX(float _NdotV, float _roughness);

// _N : normale
// _V : viewDirection
// _L : lightDirection
// _roughness : material roughness
float GeometrySmith(vec3 _N, vec3 _V, vec3 _L, float _roughness);

float ComputeAttenuation(float _distance, float _lightRange);
vec3 ComputePointLightLighting(PointLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);
vec3 ComputeDirectionalLightLighting(DirectionalLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);
vec3 ComputeSpotLightLighting(SpotLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);

vec3 ComputeLighting(Light _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);

void main() 
{
	vec3 albedo = texture(albedoSampler, fsIn.fragTexCoord.xy).xyz;
	float metalness = texture(metalnessMapSampler, fsIn.fragTexCoord.xy).x;
	float roughness = texture(roughnessMapSampler, fsIn.fragTexCoord.xy).x;
	float ao = texture(aoMapSampler, fsIn.fragTexCoord.xy).x;

	vec3 normal = normalize(fsIn.interpNormal);
    vec3 viewDirection = normalize(fsIn.cameraPos - fsIn.fragPos);

	// calculate reflectance at normal incidence;
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalness);

	vec3 Lo = vec3(0.0);

	for(int i = 0; i < pointLightsBuffer.lights.length(); ++i) 
    {
		if(pointLightsBuffer.lights[i].enable == true)
		{
			Lo += ComputePointLightLighting(pointLightsBuffer.lights[i], normal, viewDirection, albedo, metalness, roughness, F0);
		}
	}

	for(int i = 0; i < directionalLightsBuffer.lights.length(); i++)
	{
		if(directionalLightsBuffer.lights[i].enable == true)
			Lo += ComputeDirectionalLightLighting(directionalLightsBuffer.lights[i], normal, viewDirection, albedo, metalness, roughness, F0);
	}

	for(int i = 0; i < spotLightsBuffer.lights.length(); i++)
  	{
		if(spotLightsBuffer.lights[i].enable == true)
			Lo += ComputeSpotLightLighting(spotLightsBuffer.lights[i], normal, viewDirection, albedo, metalness, roughness, F0);
	}

	vec3 ambient = vec3(0.01) * albedo * ao;
    
    vec3 color = ambient + Lo;

	color  = color  / (color  + vec3(1.0));

	outColor = vec4(color , 1.0);
}

vec3 ComputePointLightLighting(PointLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	Light light;
	light.color = _light.color;
	light.direction = normalize(_light.position - fsIn.fragPos);

	float distance = length(_light.position - fsIn.fragPos);
	float attenuation = ComputeAttenuation(distance, _light.range);
	light.radiance = _light.color * _light.intensity * attenuation;

	return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
}

vec3 ComputeDirectionalLightLighting(DirectionalLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	Light light;
	light.color = _light.color;
	light.direction = normalize(-_light.direction);
	light.radiance = _light.color * _light.intensity;

	return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
}

vec3 ComputeSpotLightLighting(SpotLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	Light light;
	light.color = _light.color;
	light.direction = normalize(_light.position - fsIn.fragPos);
	float lightTheta = dot(light.direction, normalize(-_light.direction));
	
	if(lightTheta > _light.cutOff) 
	{
		float distance = length(_light.position - fsIn.fragPos);
		float attenuation = ComputeAttenuation(distance, _light.range);
		light.radiance = _light.color * _light.intensity * attenuation;

		return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
	}
	return vec3(0.0);
}

vec3 ComputeLighting(Light _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	vec3 halfwayDirection = normalize(_viewDirection + _light.direction);

	float D = DistributionGGX(_normal, halfwayDirection, _roughness);
	vec3 F = FresnelSchlick(halfwayDirection, _viewDirection, _F0);
	float G = GeometrySmith(_normal, _viewDirection, _light.direction, _roughness);

	vec3 numerator    = D * F * G; 
	float denominator = 4.0 * max(dot(_normal, _viewDirection), 0.0) * max(dot(_normal, _light.direction), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
	vec3 specular = numerator / denominator;

	// kS is equal to Fresnel
	vec3 kS = F;
	// for energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kD) should equal 1.0 - kS.
	vec3 kD = vec3(1.0) - kS;
	// multiply kD by the inverse metalness such that only non-metals 
	// have diffuse lighting, or a linear blend if partly metal (pure metals
	// have no diffuse light).
	kD *= 1.0 - _metalness;	  

	// scale light by NdotL
	float NdotL = max(dot(_normal, _light.direction), 0.0);  

	// add to outgoing radiance Lo
	return (kD * _albdeo / PI + specular) * _light.radiance * NdotL;
}

float DistributionGGX(vec3 _N, vec3 _H, float _roughness)
{
    float a = _roughness * _roughness;
    float a2 = a*a;
    float NdotH = max(dot(_N, _H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

vec3 FresnelSchlick(vec3 _H, vec3 _V, vec3 _F0)
{
    return _F0 + (1.0 - _F0) * pow(1.0 - clamp(dot(_H, _V), 0.0, 1.0), 5.0);
}

float GeometrySchlickGGX(float _NdotV, float _roughness)
{
    float nom   = _NdotV;
    float denom = _NdotV * (1.0 - _roughness) + _roughness;
	
    return nom / denom;
}
  
float GeometrySmith(vec3 _N, vec3 _V, vec3 _L, float _roughness)
{
    float NdotV = max(dot(_N, _V), 0.0);
    float NdotL = max(dot(_N, _L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, _roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, _roughness);
	
    return ggx1 * ggx2;
}

float ComputeAttenuation(float _distance, float _lightRange)
{
	float linear = 4.5 / _lightRange;
	float quadratic = 75.0 / (_lightRange * _lightRange);
	return 1.0 / (1.0 + linear * _distance + quadratic * (_distance * _distance));
	
	// quadratic attenuation
	//return 1.0 / (_distance * _distance);
}

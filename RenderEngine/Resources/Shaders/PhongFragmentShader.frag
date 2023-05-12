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
layout(set = 1, binding = 1) uniform sampler2D metalnessSampler;

layout(set = 1, binding = 4) uniform MaterialBufferObject 
{
	float shininess;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
} material;

struct PointLight
{
	bool enable;
	vec3 position;
	vec3 color;
	float intensity;
	float range;
};

layout (set = 1,binding = 5) buffer PointLightData
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

layout (set = 1,binding = 6) buffer DirectionalLightData
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

layout (set = 1,binding = 7) buffer SpotLightData
{
	SpotLight lights[];
} spotLightsBuffer;


layout(location = 0) out vec4 outColor;

struct Light
{
	vec3 color;
	vec3 direction;
	vec3 radiance;
};

float ComputeAttenuation(float _distance, float _lightRange);
vec3 ComputePointLightLighting(PointLight _pointLight, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness);
vec3 ComputeDirectionalLightLighting(DirectionalLight _directionalLight, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness);
vec3 ComputeSpotLightLighting(SpotLight _spotLight, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness);
vec3 ComputeLighting(Light _light, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness);


void main() 
{
    vec3 albedo = texture(albedoSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y)).xyz;
    float metalness = texture(metalnessSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y)).x;

	vec3 normal = normalize(fsIn.interpNormal);
    vec3 viewDirection = normalize(fsIn.cameraPos - fsIn.fragPos);

	vec3 color = vec3(0.0);
	for(int i = 0; i < pointLightsBuffer.lights.length(); i++)
	{
		if(pointLightsBuffer.lights[i].enable == true)
			color += ComputePointLightLighting(pointLightsBuffer.lights[i], normal, viewDirection, albedo, metalness);
	}

	for(int i = 0; i < directionalLightsBuffer.lights.length(); i++)
	{
		if(directionalLightsBuffer.lights[i].enable == true)
			color += ComputeDirectionalLightLighting(directionalLightsBuffer.lights[i], normal, viewDirection, albedo, metalness);
	}

	for(int i = 0; i < spotLightsBuffer.lights.length(); i++)
  	{
		if(spotLightsBuffer.lights[i].enable == true)
			color += ComputeSpotLightLighting(spotLightsBuffer.lights[i], normal, viewDirection, albedo, metalness);
	}

	// Add ambiant
	color += material.Ka * albedo;

	outColor = vec4(color, 1.0);
}

float ComputeAttenuation(float _distance, float _lightRange)
{
	float linear = 4.5 / _lightRange;
	float quadratic = 75.0 / (_lightRange * _lightRange);
	return 1.0 / (1.0 + linear * _distance + quadratic * (_distance * _distance));

	// quadratic attenuation
	//return 1.0 / (_distance * _distance);
}

vec3 ComputePointLightLighting(PointLight _pointLight, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness)
{
	Light light;
	light.color = _pointLight.color;
	light.direction = normalize(_pointLight.position - fsIn.fragPos);

	float distance = length(_pointLight.position - fsIn.fragPos);
	float attenuation = ComputeAttenuation(distance, _pointLight.range);
	light.radiance = _pointLight.color * _pointLight.intensity * attenuation;

	return ComputeLighting(light, _normal, _viewDirection, _albedo, _metalness);
}

vec3 ComputeDirectionalLightLighting(DirectionalLight _directionalLight, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness)
{
	Light light;
	light.color = _directionalLight.color;
	light.direction = normalize(-_directionalLight.direction);
	light.radiance = _directionalLight.color * _directionalLight.intensity;

	return ComputeLighting(light, _normal, _viewDirection, _albedo, _metalness);

}

vec3 ComputeSpotLightLighting(SpotLight _spotLight, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness)
{
	Light light;
	light.color = _spotLight.color;
	light.direction = normalize(_spotLight.position - fsIn.fragPos);
	float lightTheta = dot(light.direction, normalize(-_spotLight.direction));
	
	float distance = length(_spotLight.position - fsIn.fragPos);
	float attenuation = ComputeAttenuation(distance, _spotLight.range);
	light.radiance = _spotLight.color * _spotLight.intensity * attenuation;

	if(lightTheta > _spotLight.cutOff) 
	{
		return ComputeLighting(light, _normal, _viewDirection, _albedo, _metalness);
	}
	return vec3(0.0);
}

vec3 ComputeLighting(Light _light, vec3 _normal, vec3 _viewDirection, vec3 _albedo, float _metalness)
{
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

	float cosTheta = dot(_normal, _light.direction);

	// Only if the light is visible from the surface point.
	if(cosTheta > 0.0) 
	{
		diffuse = material.Kd * _albedo * cosTheta;

		vec3 reflectDirection = 2.0 * dot(_light.direction, _normal) * _normal - _light.direction;  

		specular = material.Ks * _albedo * pow(max(dot(_viewDirection, reflectDirection), 0.0), material.shininess) * _metalness;
	}

	return _light.radiance * (diffuse + specular);
}
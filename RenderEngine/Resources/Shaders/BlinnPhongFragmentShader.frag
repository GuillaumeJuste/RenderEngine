#version 450
#pragma shader_stage(fragment)

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} fsIn;

layout(set = 1, binding = 0) uniform sampler2D albedoSampler;
layout(set = 1, binding = 1) uniform sampler2D specularSampler;

layout(set = 1, binding = 2) uniform MaterialBufferObject 
{
	float shininess;
	vec4 Ka;
	vec4 Kd;
	vec4 Ks;
} material;

struct PointLight
{
	bool enable;
	vec3 position;
	vec3 color;
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
	float range;
	float cutOff;
	
};

layout (set = 1,binding = 5) buffer SpotLightData
{
	SpotLight lights[];
} spotLightsBuffer;


layout(location = 0) out vec4 outColor;

float ComputeAttenuation(float _distance, float _lightRange);
vec4 ComputePointLightLighting(PointLight _pointLight,vec4 _albedo, vec4 _metalness);
vec4 ComputeDirectionalLightLighting(DirectionalLight _directionalLight, vec4 _albedo, vec4 _metalness);
vec4 ComputeSpotLightLighting(SpotLight _spotLight, vec4 _albedo, vec4 _metalness);

void main() 
{
    vec4 albedo = texture(albedoSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));
    vec4 specular = texture(specularSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));

	vec4 color = vec4(0.0);
	for(int i = 0; i < pointLightsBuffer.lights.length(); i++)
	{
		if(pointLightsBuffer.lights[i].enable == true)
			color += ComputePointLightLighting(pointLightsBuffer.lights[i], albedo, specular);
	}

	for(int i = 0; i < directionalLightsBuffer.lights.length(); i++)
	{
		if(directionalLightsBuffer.lights[i].enable == true)
			color += ComputeDirectionalLightLighting(directionalLightsBuffer.lights[i], albedo, specular);
	}

	for(int i = 0; i < spotLightsBuffer.lights.length(); i++)
  	{
		if(spotLightsBuffer.lights[i].enable == true)
			color += ComputeSpotLightLighting(spotLightsBuffer.lights[i], albedo, specular);
	}

	outColor = color;
}

float ComputeAttenuation(float _distance, float _lightRange)
{
	float linear = 4.5 / _lightRange;
	float quadratic = 75.0 / (_lightRange * _lightRange);
	return 1.0 / (1.0 + linear * _distance + quadratic * (_distance * _distance));
}

vec4 ComputePointLightLighting(PointLight _pointLight,vec4 _albedo, vec4 _metalness)
{
	float distance = length(_pointLight.position - fsIn.fragPos);

	float attenuation = ComputeAttenuation(distance, _pointLight.range);    

	vec4 ambient = vec4(_pointLight.color.xyz, 1.0) * material.Ka * _albedo;

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	vec3 normal = normalize(fsIn.normal);
	vec3 lightDirection = normalize(_pointLight.position - fsIn.fragPos);  

	float cosTheta = max(dot(normal, lightDirection), 0.0);

	// Only if the light is visible from the surface point.
	if(cosTheta > 0.0) 
	{
		diffuse = material.Kd * _albedo * vec4(_pointLight.color.xyz, 1.0) * cosTheta;

		vec3 viewDirection = normalize(fsIn.cameraPos - fsIn.fragPos);
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);

		specular = material.Ks * _albedo * pow(max(dot(normal, halfwayDirection), 0.0), material.shininess) * vec4(_pointLight.color.xyz, 1.0) * _metalness;
	}

	return attenuation * (ambient + diffuse + specular);
}

vec4 ComputeDirectionalLightLighting(DirectionalLight _directionalLight, vec4 _albedo, vec4 _metalness)
{
	vec4 ambient = vec4(_directionalLight.color.xyz, 1.0) * material.Ka * _albedo;

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	vec3 normal = normalize(fsIn.normal);
	vec3 lightDirection = normalize(-_directionalLight.direction);

	float cosTheta = max(dot(normal, lightDirection), 0.0);

	// Only if the light is visible from the surface point.
	if(cosTheta > 0.0) 
	{
		diffuse = material.Kd * _albedo * vec4(_directionalLight.color.xyz, 1.0) * cosTheta;

		vec3 viewDirection = normalize(fsIn.cameraPos - fsIn.fragPos);
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);
  
		specular = material.Ks * _albedo * pow(max(dot(normal, halfwayDirection), 0.0), material.shininess) * vec4(_directionalLight.color.xyz, 1.0) * _metalness;
	}

	return ambient + diffuse + specular;

}

vec4 ComputeSpotLightLighting(SpotLight _spotLight, vec4 _albedo, vec4 _metalness)
{
	vec3 lightDirection = normalize(_spotLight.position - fsIn.fragPos);   
	float lightTheta = dot(lightDirection, normalize(-_spotLight.direction));

	vec4 ambient = vec4(_spotLight.color.xyz, 1.0) * material.Ka * _albedo;
    
	float distance    = length(_spotLight.position - fsIn.fragPos);
	float attenuation = ComputeAttenuation(distance, _spotLight.range);    

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	if(lightTheta > _spotLight.cutOff) 
	{       
	vec3 normal = normalize(fsIn.normal);
	float cosTheta = max(dot(normal, lightDirection), 0.0);

		// Only if the light is visible from the surface point.
		if(cosTheta > 0.0) 
		{
			diffuse = material.Kd * _albedo * vec4(_spotLight.color.xyz, 1.0) * cosTheta;

			vec3 viewDirection = normalize(fsIn.cameraPos - fsIn.fragPos);
			vec3 halfwayDirection = normalize(lightDirection + viewDirection);

			specular = material.Ks * _albedo * pow(max(dot(normal, halfwayDirection), 0.0), material.shininess) * vec4(_spotLight.color.xyz, 1.0) * _metalness;
		}
	}
	return attenuation * (ambient + diffuse + specular);
}
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

layout(set = 1, binding = 2) uniform MaterialBufferObject {
	float shininess;
} material;

struct PointLight
{
	bool enable;
	vec3 position;
	vec3 color;
	float range;
	float ambient;
	float diffuse;
	float specular;
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
	float ambient;
	float diffuse;
	float specular;
};

layout (set = 1,binding = 4) buffer DirectionalLightData
{
	DirectionalLight lights[];
} directionalLightsBuffer;

struct SpotLight
{
	bool enable;
	vec3 position;
	vec3 direction;
	vec3 color;
	float range;
	float cutOff;
	float ambient;
	float diffuse;
	float specular;
};

layout (set = 1,binding = 5) buffer SpotLightData
{
	SpotLight lights[];
} spotLightsBuffer;


layout(location = 0) out vec4 outColor;

vec3 ComputePointLightLighting(PointLight _pointLight, vec3 _specular);
vec3 ComputeDirectionalLightLighting(DirectionalLight _directionalLight, vec3 _specular);
vec3 ComputeSpotLightLighting(SpotLight _spotLight, vec3 _specular);

void main() 
{
    vec4 albedo = texture(albedoSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));
    vec4 specular = texture(specularSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));

	vec3 color = vec3(0.0);
    for(int i = 0; i < pointLightsBuffer.lights.length(); i++)
  	{
		if(pointLightsBuffer.lights[i].enable == true)
			color += ComputePointLightLighting(pointLightsBuffer.lights[i], vec3(specular));
	}

	for(int i = 0; i < directionalLightsBuffer.lights.length(); i++)
  	{
		if(directionalLightsBuffer.lights[i].enable == true)
			color += ComputeDirectionalLightLighting(directionalLightsBuffer.lights[i], vec3(specular));
	}

	for(int i = 0; i < spotLightsBuffer.lights.length(); i++)
  	{
		if(spotLightsBuffer.lights[i].enable == true)
			color += ComputeSpotLightLighting(spotLightsBuffer.lights[i], vec3(specular));
	}

	outColor = albedo * vec4(color, 1.0);
}

vec3 ComputePointLightLighting(PointLight _pointLight, vec3 _specular)
{
	float distance    = length(_pointLight.position - fsIn.fragPos);
	float linear = 4.5 / _pointLight.range;
	float quadratic = 75.0 / (_pointLight.range * _pointLight.range);
	float attenuation = 1.0 / (1.0 + linear * distance + 
		quadratic * (distance * distance));    

	vec3 ambient = _pointLight.color * _pointLight.ambient ;

	vec3 norm = normalize(fsIn.normal);
	vec3 lightDir = normalize(_pointLight.position - fsIn.fragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = _pointLight.color * _pointLight.diffuse * diff;

	vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
	vec3 specular = _pointLight.color * _pointLight.specular * spec * _specular;  

	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   

	return ambient + diffuse + specular;
}

vec3 ComputeDirectionalLightLighting(DirectionalLight _directionalLight, vec3 _specular)
{
	vec3 ambient = _directionalLight.color * _directionalLight.ambient;

	vec3 norm = normalize(fsIn.normal);
	vec3 lightDir = normalize(-_directionalLight.direction);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = _directionalLight.color * _directionalLight.diffuse * diff;

	vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
	vec3 specular = _directionalLight.color * _directionalLight.specular * spec * _specular;  

	return ambient + diffuse + specular;
}

vec3 ComputeSpotLightLighting(SpotLight _spotLight, vec3 _specular)
{
	vec3 lightDir = normalize(_spotLight.position - fsIn.fragPos);  
	float theta = dot(lightDir, normalize(-_spotLight.direction));
	vec3 ambient = _spotLight.color * _spotLight.ambient;
    
	float distance    = length(_spotLight.position - fsIn.fragPos);
	float linear = 4.5 / _spotLight.range;
	float quadratic = 75.0 / (_spotLight.range * _spotLight.range);
	float attenuation = 1.0 / (1.0 + linear * distance + 
		quadratic * (distance * distance));    
	
	ambient  *= attenuation; 

	if(theta > _spotLight.cutOff) 
	{       
		vec3 norm = normalize(fsIn.normal);

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = _spotLight.color * _spotLight.diffuse * diff;

		vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);

		float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
		vec3 specular = _spotLight.color * _spotLight.specular * spec * _specular;  

		diffuse  *= attenuation;
		specular *= attenuation;   

		return ambient + diffuse + specular;
	}

	return ambient;
}
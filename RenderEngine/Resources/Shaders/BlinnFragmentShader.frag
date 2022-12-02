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

vec3 ComputePointLightLighting(PointLight pointLight, vec3 specularMap);
vec3 ComputeDirectionalLightLighting(DirectionalLight directionalLight, vec3 specularMap);
vec3 ComputeSpotLightLighting(SpotLight spotLight, vec3 specularMap);

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

vec3 ComputePointLightLighting(PointLight pointLight, vec3 specularMap)
{
	float distance    = length(pointLight.position - fsIn.fragPos);
	float linear = 4.5 / pointLight.range;
	float quadratic = 75.0 / (pointLight.range * pointLight.range);
	float attenuation = 1.0 / (1.0 + linear * distance + 
		quadratic * (distance * distance));    

	vec3 ambient = pointLight.color * pointLight.ambient;

	vec3 norm = normalize(fsIn.normal);
	vec3 lightDir = normalize(pointLight.position - fsIn.fragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLight.color * pointLight.diffuse * diff;

	vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pointLight.color * pointLight.specular * spec * specularMap;  

	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   

	return ambient + diffuse + specular;
}

vec3 ComputeDirectionalLightLighting(DirectionalLight directionalLight, vec3 specularMap)
{
	vec3 ambient = directionalLight.color * directionalLight.ambient;

	vec3 norm = normalize(fsIn.normal);
	vec3 lightDir = normalize(-directionalLight.direction);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = directionalLight.color * directionalLight.diffuse * diff;

	vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = directionalLight.color * directionalLight.specular * spec * specularMap;  

	return ambient + diffuse + specular;
}

vec3 ComputeSpotLightLighting(SpotLight spotLight, vec3 specularMap)
{
	vec3 lightDir = normalize(spotLight.position - fsIn.fragPos);  
	float theta = dot(lightDir, normalize(-spotLight.direction));
	vec3 ambient = spotLight.color * spotLight.ambient;
    
	float distance    = length(spotLight.position - fsIn.fragPos);
	float linear = 4.5 / spotLight.range;
	float quadratic = 75.0 / (spotLight.range * spotLight.range);
	float attenuation = 1.0 / (1.0 + linear * distance + 
		quadratic * (distance * distance));    
	
	ambient  *= attenuation; 

	if(theta > spotLight.cutOff) 
	{       
		vec3 norm = normalize(fsIn.normal);

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = spotLight.color * spotLight.diffuse * diff;

		vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = spotLight.color * spotLight.specular * spec * specularMap;  

		diffuse  *= attenuation;
		specular *= attenuation;   

		return ambient + diffuse + specular;
	}

	return ambient;
}
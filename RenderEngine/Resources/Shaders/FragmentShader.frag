#version 450
#pragma shader_stage(fragment)

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} fsIn;

layout(binding = 2) uniform sampler2D albedoSampler;
layout(binding = 5) uniform sampler2D specularSampler;

layout(binding = 3) uniform MaterialBufferObject {
	vec3 specular;
	float shininess;
} material;

struct Light
{
	vec3 position;
	vec3 color;
	float ambient;
	float diffuse;
	float specular;
};

layout (binding = 4) buffer LightInfo
{
	Light lights[];
} lightsBuffer;

layout(location = 0) out vec4 outColor;

vec3 ComputeLighting(Light light, vec3 texture, vec3 specularMap);

void main() 
{
    vec4 albedo = texture(albedoSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));
    vec4 specular = texture(specularSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));

	vec3 color = vec3(0.0);
    for(int i = 0; i < lightsBuffer.lights.length(); i++)
  		color += ComputeLighting(lightsBuffer.lights[i], vec3(albedo), vec3(specular));

	outColor = vec4(color, 1.0);
}

vec3 ComputeLighting(Light light, vec3 texture, vec3 specularMap)
{
	vec3 ambient = light.color * light.ambient * texture;

	vec3 norm = normalize(fsIn.normal);
	vec3 lightDir = normalize(light.position - fsIn.fragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.color * light.diffuse * diff * texture;

	vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.color * light.specular * spec * specularMap;  

	return ambient + diffuse + specular;
}
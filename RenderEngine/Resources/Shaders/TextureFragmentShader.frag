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
layout(set = 1, binding = 3) uniform sampler2D specularSampler;

layout(set = 1, binding = 1) uniform MaterialBufferObject {
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

layout (set = 1,binding = 2) buffer PointLightData
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

void main() 
{
    outColor = texture(albedoSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));
}
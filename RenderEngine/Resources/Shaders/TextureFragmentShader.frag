#version 450

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} fsIn;

layout(binding = 2) uniform sampler2D texSampler;

layout(binding = 3) uniform MaterialBufferObject {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

struct Light
{
	vec3 position;
	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (binding = 4) buffer LightInfo
{
	Light lights[];
} lightsBuffer;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));
}
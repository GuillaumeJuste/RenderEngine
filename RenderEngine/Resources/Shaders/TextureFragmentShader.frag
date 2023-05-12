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

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = texture(albedoSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));
}
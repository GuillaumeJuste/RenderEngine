#version 450
#pragma shader_stage(fragment)

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 texCoord;
	vec3 cameraPos;
    vec3 tangent;
} fsIn;

layout(set = 1, binding = 0) uniform sampler2D albedoSampler;

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = texture(albedoSampler, fsIn.texCoord.xy);
}
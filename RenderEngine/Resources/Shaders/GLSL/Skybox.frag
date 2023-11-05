#version 450

layout(set = 1, binding = 0) uniform samplerCube samplerCubeMap;

layout(location = 0) in DataBlock
{
	vec3 UVW;
} vsIn;

layout(location = 0) out vec4 outColor;

void main() 
{
	outColor = texture(samplerCubeMap, vsIn.UVW);
	//outColor = vec4(vsIn.UVW.z);
}
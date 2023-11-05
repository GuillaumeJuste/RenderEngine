#version 450
#pragma shader_stage(vertex)

layout(push_constant) uniform PushConsts 
{
    layout (offset = 0) mat4 invView;
    layout (offset = 64) mat4 proj;
} cbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTexCoord;
layout(location = 3) in vec3 inTangent;

layout(location = 0) out DataBlock
{
	vec3 UVW;
} vsOut;

void main() 
{
	vsOut.UVW = inPosition;

	mat4 staticView = mat4(mat3(cbo.invView)); // Remove camera translation.
	gl_Position = cbo.proj * staticView * vec4(inPosition, 1.0);
}
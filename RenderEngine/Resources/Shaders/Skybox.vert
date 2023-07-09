#version 450
#pragma shader_stage(vertex)

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 invView;
    mat4 proj;
    vec3 cameraPos;
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
	gl_Position = gl_Position.xyww;
}
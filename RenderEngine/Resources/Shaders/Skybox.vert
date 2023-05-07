#version 450
#pragma shader_stage(vertex)

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 invView;
    mat4 proj;
    vec3 cameraPos;
} cbo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out DataBlock
{
	vec3 UVW;
} vsOut;

void main() 
{
	vsOut.UVW = inPosition;
	vsOut.UVW.xy *= -1.0;

	gl_Position = cbo.proj * cbo.invView * vec4(inPosition.xyz, 1.0);
}
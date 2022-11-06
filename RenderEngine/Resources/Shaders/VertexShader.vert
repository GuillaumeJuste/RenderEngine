#version 450
#pragma shader_stage(vertex)

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
} ubo;

layout(binding = 1) uniform CameraBufferObject {
    mat4 invView;
    mat4 proj;
    vec3 cameraPos;
} cbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTexCoord;

layout(location = 0) out DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} vsOut;

void main() {
    gl_Position = cbo.proj * cbo.invView * ubo.model * vec4(vec3(inPosition.x, inPosition.y, inPosition.z), 1.0);

    vsOut.fragTexCoord = inTexCoord;
    vsOut.fragPos = mat3(ubo.model) * inPosition;
    vsOut.normal = mat3(transpose(inverse(ubo.model))) * inNormal;  
    vsOut.cameraPos = cbo.cameraPos;
}
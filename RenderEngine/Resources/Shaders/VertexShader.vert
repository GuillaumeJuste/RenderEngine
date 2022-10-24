#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 2) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
} cbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTexCoord;

layout(location = 0) out vec3 fragTexCoord;

void main() {
    gl_Position = cbo.proj * cbo.view * ubo.model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}
#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, vec2(fragTexCoord.x, fragTexCoord.y));
}
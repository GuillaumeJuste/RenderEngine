#version 450
#pragma shader_stage(vertex)

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
} ubo;

layout(set = 0, binding = 1) uniform CameraBufferObject {
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
	vec3 fragPos;
	vec3 interpNormal;
	vec3 fragTexCoord;
	vec3 cameraPos;
    vec3 tangent;
} vsOut;

void main() {
    gl_Position = cbo.proj * cbo.invView * ubo.model * vec4(vec3(inPosition.x, inPosition.y, inPosition.z), 1.0);

    vsOut.fragTexCoord = inTexCoord;
    vsOut.fragPos = vec3( ubo.model * vec4(inPosition, 1.0));
    //vsOut.fragPos = vec3(gl_Position);

    mat3 modelMat3 = mat3(transpose(inverse(ubo.model)));
    vsOut.interpNormal = modelMat3 * inNormal;  
    vsOut.tangent = modelMat3 * inTangent;
    vsOut.cameraPos = cbo.cameraPos;
}
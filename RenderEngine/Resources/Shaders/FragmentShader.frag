#version 450

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} fsIn;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

vec3 lightColor = vec3(1.0, 0.42, 0);
vec3 lightPosition = vec3(0.0, 0.0, -3.0);
float ambientStrength = 0.1;
float specularStrength = 0.8;

void main() {
    vec4 texture = texture(texSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));

    vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(fsIn.normal);
	vec3 lightDir = normalize(lightPosition - fsIn.fragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	outColor =  texture * vec4(ambient + diffuse + specular, 1.0);
}
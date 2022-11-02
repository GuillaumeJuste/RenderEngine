#version 450

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} fsIn;

layout(binding = 2) uniform sampler2D texSampler;

layout(binding = 3) uniform MaterialBufferObject {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

layout(location = 0) out vec4 outColor;

vec3 lightColor = vec3(1.0, 0.42, 0);
vec3 lightPosition = vec3(0.0, 0.0, -3.0);
float ambientStrength = 0.1;
float specularStrength = 0.8;

void main() {
    vec4 texture = texture(texSampler, vec2(fsIn.fragTexCoord.x, fsIn.fragTexCoord.y));

    vec3 ambient = ambientStrength * lightColor * material.ambient;

	vec3 norm = normalize(fsIn.normal);
	vec3 lightDir = normalize(lightPosition - fsIn.fragPos);  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * (diff * material.diffuse);

	vec3 viewDir = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor * (spec * material.specular);  

	outColor =  texture * vec4(ambient + diffuse + specular, 1.0);
}
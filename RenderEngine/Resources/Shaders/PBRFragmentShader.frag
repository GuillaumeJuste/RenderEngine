#version 450
#pragma shader_stage(fragment)

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 normal;
	vec3 fragTexCoord;
	vec3 cameraPos;
} fsIn;

layout(set = 1, binding = 0) uniform sampler2D albedoSampler;
layout(set = 1, binding = 1) uniform sampler2D specularSampler;

layout(set = 1, binding = 2) uniform MaterialBufferObject {
	float shininess;
} material;

struct PointLight
{
	bool enable;
	vec3 position;
	vec3 color;
	float range;
	float ambient;
	float diffuse;
	float specular;
};

layout (set = 1,binding = 3) buffer PointLightData
{
	PointLight lights[];
} pointLightsBuffer;

struct DirectionalLight
{
	bool enable;
	vec3 color;
	vec3 direction;
	float ambient;
	float diffuse;
	float specular;
};

layout (set = 1,binding = 4) buffer DirectionalLightData
{
	DirectionalLight lights[];
} directionalLightsBuffer;

struct SpotLight
{
	bool enable;
	vec3 position;
	vec3 direction;
	vec3 color;
	float range;
	float cutOff;
	float ambient;
	float diffuse;
	float specular;
};

layout (set = 1,binding = 5) buffer SpotLightData
{
	SpotLight lights[];
} spotLightsBuffer;


layout(location = 0) out vec4 outColor;

float roughness = 0.2;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec3 ComputeLighting(PointLight light, vec3 _albedo, vec3 _metallic);

void main() 
{
	vec3 color = vec3(0.0);

	vec3 albedo = texture(albedoSampler, fsIn.fragTexCoord.xy).xyz;
	vec3 metallic = texture(specularSampler, fsIn.fragTexCoord.xy).xyz;

	for(int i = 0; i < pointLightsBuffer.lights.length(); i++)
  		color += ComputeLighting(pointLightsBuffer.lights[i], albedo, metallic);

	color = color / (color + vec3(1.0));

	outColor = vec4(color, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

vec3 ComputeLighting(PointLight light, vec3 _albedo, vec3 _metallic)
{
    vec3 N = normalize(fsIn.normal);
    vec3 V = normalize(fsIn.cameraPos - fsIn.fragPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, _albedo, _metallic);
	           
	// calculate light radiance
	vec3 L = normalize(light.position - fsIn.fragPos);
	vec3 H = normalize(V + L);
	float distance    = length(light.position - fsIn.fragPos);
	float attenuation = 1.0;
	vec3 radiance     = light.color * attenuation;        
        
    // cook-torrance brdf
	float NDF = DistributionGGX(N, H, roughness);        
	float G   = GeometrySmith(N, V, L, roughness);      
	vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - _metallic;	  
        
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular     = numerator / max(denominator, 0.001);  
            
	float NdotL = max(dot(N, L), 0.0);                
	vec3 Lo = (kD * _albedo / PI + specular) * radiance * NdotL; 
  
    vec3 ambient = vec3(0.03) * _albedo * light.ambient;
    vec3 color = ambient + Lo;
   
    return color;
}
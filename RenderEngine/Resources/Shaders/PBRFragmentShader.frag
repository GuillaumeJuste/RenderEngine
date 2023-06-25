#version 450
#pragma shader_stage(fragment)

layout(location = 0) in DataBlock
{
	vec3 fragPos;
	vec3 interpNormal;
	vec3 fragTexCoord;
	vec3 cameraPos;
    vec3 uvw;
} fsIn;

layout(set = 1, binding = 0) uniform sampler2D albedoSampler;
layout(set = 1, binding = 1) uniform sampler2D metalnessMapSampler;
layout(set = 1, binding = 2) uniform sampler2D roughnessMapSampler;
layout(set = 1, binding = 3) uniform sampler2D aoMapSampler;

struct PointLight
{
	bool enable;
	vec3 position;
	vec3 color;
	float intensity;
	float range;
};

layout (set = 1,binding = 5) buffer PointLightData
{
	PointLight lights[];
} pointLightsBuffer;

struct DirectionalLight
{
	bool enable;
	vec3 color;
	vec3 direction;
	float intensity;
};

layout (set = 1,binding = 6) buffer DirectionalLightData
{
	DirectionalLight lights[];
} directionalLightsBuffer;

struct SpotLight
{
	bool enable;
	vec3 position;
	vec3 color;
	vec3 direction;
	float intensity;
	float range;
	float cutOff;
	
};

layout (set = 1,binding = 7) buffer SpotLightData
{
	SpotLight lights[];
} spotLightsBuffer;

layout(set = 1, binding = 8) uniform samplerCube irradianceSampler;
layout(set = 1, binding = 9) uniform samplerCube prefilterSampler;
layout(set = 1, binding = 10) uniform sampler2D BRDFlutSampler;
layout(set = 1, binding = 11) uniform samplerCube skyboxSampler;

layout(location = 0) out vec4 outColor;

float roughness = 0.2;

const float PI = 3.14159265359;

struct Light
{
	vec3 color;
	vec3 direction;
	vec3 radiance;
};


// _N : normal
// _H : halfwayDirection
// _roughness : material roughness from map
float DistributionGGX(vec3 _N, vec3 _H, float _roughness);

// _H : halfwayDirection
// _V : viewDirection
// _F0 : reflectance at normal incidence
vec3 FresnelSchlickRoughness(float _cosTheta, vec3 _F0, float _roughness);

// _NdotV : normal and  view dot product
// _roughness : material roughness
float GeometrySchlickGGX(float _NdotV, float _roughness);

// _N : normale
// _V : viewDirection
// _L : lightDirection
// _roughness : material roughness
float GeometrySmith(vec3 _N, vec3 _V, vec3 _L, float _roughness);

float ComputeAttenuation(float _distance, float _lightRange);
vec3 ComputePointLightLighting(PointLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);
vec3 ComputeDirectionalLightLighting(DirectionalLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);
vec3 ComputeSpotLightLighting(SpotLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);

vec3 ComputeLighting(Light _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0);

vec3 computeIrradiance();
vec3 computePrefiltered();

void main() 
{
	vec3 albedo = texture(albedoSampler, fsIn.fragTexCoord.xy).xyz;
	float metalness = texture(metalnessMapSampler, fsIn.fragTexCoord.xy).x;
	float roughness = texture(roughnessMapSampler, fsIn.fragTexCoord.xy).x;
	float ao = texture(aoMapSampler, fsIn.fragTexCoord.xy).x;

	vec3 normal = normalize(fsIn.interpNormal);
    vec3 viewDirection = normalize(fsIn.cameraPos - fsIn.fragPos);
	vec3 reflection = reflect(-viewDirection, normal); 

	// calculate reflectance at normal incidence;
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalness);

	vec3 Lo = vec3(0.0);

	for(int i = 0; i < pointLightsBuffer.lights.length(); ++i) 
    {
		if(pointLightsBuffer.lights[i].enable == true)
		{
			Lo += ComputePointLightLighting(pointLightsBuffer.lights[i], normal, viewDirection, albedo, metalness, roughness, F0);
		}
	}

	for(int i = 0; i < directionalLightsBuffer.lights.length(); i++)
	{
		if(directionalLightsBuffer.lights[i].enable == true)
			Lo += ComputeDirectionalLightLighting(directionalLightsBuffer.lights[i], normal, viewDirection, albedo, metalness, roughness, F0);
	}

	for(int i = 0; i < spotLightsBuffer.lights.length(); i++)
  	{
		if(spotLightsBuffer.lights[i].enable == true)
			Lo += ComputeSpotLightLighting(spotLightsBuffer.lights[i], normal, viewDirection, albedo, metalness, roughness, F0);
	}

	vec3 kS = FresnelSchlickRoughness(max(dot(normal, viewDirection), 0.0), F0, roughness);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metalness;	  
	vec3 irradiance = texture(irradianceSampler, normal).rgb;
    //vec3 irradiance = computeIrradiance();
    vec3 diffuse = irradiance * albedo;

    vec3 prefilteredColor = texture(prefilterSampler, reflection).rgb;    
    //vec3 prefilteredColor = computePrefiltered();    
    vec2 brdf  = texture(BRDFlutSampler, vec2(max(dot(normal, viewDirection), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

	color  = color  / (color  + vec3(1.0));

	outColor = vec4(color , 1.0);
}

vec3 ComputePointLightLighting(PointLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	Light light;
	light.color = _light.color;
	light.direction = normalize(_light.position - fsIn.fragPos);

	float distance = length(_light.position - fsIn.fragPos);
	float attenuation = ComputeAttenuation(distance, _light.range);
	light.radiance = _light.color * _light.intensity * attenuation;

	return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
}

vec3 ComputeDirectionalLightLighting(DirectionalLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	Light light;
	light.color = _light.color;
	light.direction = normalize(-_light.direction);
	light.radiance = _light.color * _light.intensity;

	return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
}

vec3 ComputeSpotLightLighting(SpotLight _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	Light light;
	light.color = _light.color;
	light.direction = normalize(_light.position - fsIn.fragPos);
	float lightTheta = dot(light.direction, normalize(-_light.direction));
	
	if(lightTheta > _light.cutOff) 
	{
		float distance = length(_light.position - fsIn.fragPos);
		float attenuation = ComputeAttenuation(distance, _light.range);
		light.radiance = _light.color * _light.intensity * attenuation;

		return ComputeLighting(light, _normal, _viewDirection, _albdeo, _metalness, _roughness, _F0);
	}
	return vec3(0.0);
}

vec3 ComputeLighting(Light _light, vec3 _normal, vec3 _viewDirection, vec3 _albdeo, float _metalness, float _roughness, vec3 _F0)
{
	vec3 halfwayDirection = normalize(_viewDirection + _light.direction);

	float D = DistributionGGX(_normal, halfwayDirection, _roughness);
	vec3 F = FresnelSchlickRoughness(max(dot(halfwayDirection, _viewDirection), 0.0), _F0, _roughness);
	float G = GeometrySmith(_normal, _viewDirection, _light.direction, _roughness);

	vec3 numerator    = D * F * G; 
	float denominator = 4.0 * max(dot(_normal, _viewDirection), 0.0) * max(dot(_normal, _light.direction), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - _metalness;	  

	// scale light by NdotL
	float NdotL = max(dot(_normal, _light.direction), 0.0);  

	// add to outgoing radiance Lo
	return (kD * _albdeo / PI + specular) * _light.radiance * NdotL;
}

float DistributionGGX(vec3 _N, vec3 _H, float _roughness)
{
    float a = _roughness * _roughness;
    float a2 = a*a;
    float NdotH = max(dot(_N, _H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

vec3 FresnelSchlickRoughness(float _cosTheta, vec3 _F0, float _roughness)
{
    return _F0 + (max(vec3(1.0 - _roughness), _F0) - _F0) * pow(clamp(1.0 - _cosTheta, 0.0, 1.0), 5.0);
}

float GeometrySchlickGGX(float _NdotV, float _roughness)
{
    float nom   = _NdotV;
    float denom = _NdotV * (1.0 - _roughness) + _roughness;
	
    return nom / denom;
}
  
float GeometrySmith(vec3 _N, vec3 _V, vec3 _L, float _roughness)
{
    float NdotV = max(dot(_N, _V), 0.0);
    float NdotL = max(dot(_N, _L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, _roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, _roughness);
	
    return ggx1 * ggx2;
}

float ComputeAttenuation(float _distance, float _lightRange)
{
	float linear = 4.5 / _lightRange;
	float quadratic = 75.0 / (_lightRange * _lightRange);
	return 1.0 / (1.0 + linear * _distance + quadratic * (_distance * _distance));
	
	// quadratic attenuation
	//return 1.0 / (_distance * _distance);
}

vec3 computeIrradiance()
{
	vec3 irradiance = vec3(0.0);  
	vec3 N = normalize(fsIn.fragPos);

	vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 normal = normalize(fsIn.interpNormal);
	vec3 right = normalize(cross(up, normal));
	up         = normalize(cross(normal, right));
	
	float sampleDelta = 0.025;
	float nrSamples = 0.0; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
	    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
	    {
	        // spherical to cartesian (in tangent space)
	        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
	        // tangent space to world
	        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 
	
	        irradiance += texture(skyboxSampler, sampleVec).rgb * cos(theta) * sin(theta);
	        nrSamples++;
	    }
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
	return irradiance;
}

float RadicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}
// ----------------------------------------------------------------------------
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness*roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
	// from spherical coordinates to cartesian coordinates - halfway vector
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	// from tangent-space H vector to world-space sample vector
	vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}
// ----------------------------------------------------------------------------
vec3 computePrefiltered()
{		
    vec3 N = normalize(fsIn.fragPos);
    
    // make the simplifying assumption that V equals R equals the normal 
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;
    
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            // sample from the environment's mip level based on roughness/pdf
            float D   = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 

            float resolution = 512.0; // resolution of source cubemap (per face)
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
            
            prefilteredColor += textureLod(skyboxSampler, L, mipLevel).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    return prefilteredColor;
}
#ifndef RENDERENGINE_SHADER_NORMAL
#define RENDERENGINE_SHADER_NORMAL

float3 getNormalFromMap(float2 _textCoord, float3 _normal, float3 _tangent, Texture2D<float4> _normalMap, SamplerState _normalMapSampler)
{
    float3 N = normalize(_normal);
    float3 T = normalize(_tangent);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T.x, B.x, N.x,
                            T.y, B.y, N.y,
                            T.z, B.z, N.z);
    
    return normalize(mul(TBN, (_normalMap.Sample(_normalMapSampler, _textCoord).rgb * 2.0 - 1.0)));
}

#endif
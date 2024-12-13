//render a particle

cbuffer ParticleBuffer : register(b0)
{
    matrix wvp;
}

Texture2D particleTexture : register(t0);
SamplerState texSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
    int4 blendIndices : BLENDINDICES;
    int4 blendWeight : BLENDWEIGHT;
    float4 id : INSTANCEPOS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position + input.id.xyz, 1.0f), wvp);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return particleTexture.Sample(texSampler, input.texCoord);
}
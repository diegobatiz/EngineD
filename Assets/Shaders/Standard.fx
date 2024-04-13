//Standard effect for rendering objects

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_OUTPUT
{
	float3 position : SV_Position;
    float3 normal : NORMAL;
	float3 tangent: TANGENT;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(float3 position : POSITION, float4 texCoord : TEXCOORD)
{
	VS_OUTPUT output;
    output.position = mul(float4(position, 1.0f), wvp);
	output.texCoord = texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return textureMap.Sample(textureSampler, input.texCoord);
}
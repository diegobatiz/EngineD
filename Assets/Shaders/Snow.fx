//Description: Fog Effect

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

Texture2D positionMap : register(t0);

SamplerState texSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.color = input.color;
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 displacement = positionMap.Sample(texSampler, input.texCoord);
    
    if (displacement.r != 0.0)
    {
        return float4(0.0, 0.0, 0.0, 0.0);
    }
    
    return input.color;
}

//Description: Shadow Effect that generates a depth map to a render target

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 depth : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.depth = output.position.z / output.position.w;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float depth = input.depth;
    return float4(depth, depth, depth, 1.0f);
}

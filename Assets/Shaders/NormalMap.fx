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
    float3 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.depth = output.position.z / output.position.w;
    output.color = input.normal * 0.5f + 0.5f;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(input.color, 1.0f);
}

//Description: Shadow Effect that generates a depth map to a render target

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

cbuffer PlayerPositionBuffer : register(b1)
{
    float2 playerPosition; //player position in texture space
}

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    
}

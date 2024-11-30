//Description: Position Map -> maps out the player's position

cbuffer PlayerPositionBuffer : register(b0)
{
    float2 playerPosition; //player position in texture space
    float playerRadius;
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
    output.position = float4(input.position, 1.0);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float distance = length(input.texCoord - playerPosition);
    
    float4 color = (distance <= playerRadius) ? float4(1.0, 0.0, 0.0, 1.0) : float4(0.0, 0.0, 0.0, 1.0);
    
    return color;
}

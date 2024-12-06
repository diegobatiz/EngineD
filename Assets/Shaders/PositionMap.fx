//Description: Position Map -> maps out the player's position

cbuffer PlayerPositionBuffer : register(b0)
{
    float2 playerPosition; //player position in texture space
    float playerRadius;
}

cbuffer TrailBuffer : register(b1)
{
    float startGradient;
    float edgeThickness;
}

Texture2D snowHeightMap : register(t0);
SamplerState texSampler : register(s0);

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
   
    float distanceFromCenter = saturate(distance / playerRadius);
    
    float snowDisplacement = 1.0 - smoothstep(startGradient, startGradient + edgeThickness, distanceFromCenter);
    
    float currentHeight = snowHeightMap.Sample(texSampler, input.texCoord).r;
    
    snowDisplacement = max(snowDisplacement, currentHeight);
    
    return float4(snowDisplacement, 0.0, 0.0, 1.0);
}

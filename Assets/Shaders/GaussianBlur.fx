//this will create a gaussian blur effect

cbuffer SettingsBuffer : register(b0)
{
    float screenWidth;
    float screenHeight;
    float multiplier;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

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
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float4 HorizontalBlurPS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor;
    
    return finalColor;
}

float4 VerticalBlurPS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor;
    
    return finalColor;
}

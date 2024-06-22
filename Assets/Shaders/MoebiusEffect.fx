//this does post processing effects

Texture2D crossHatchTexture : register(t0);
Texture2D comicTexture : register(t1);
Texture2D shadowTexture : register(t2);

SamplerState textureSampler : register(s0);

cbuffer ComicBuffer : register(b0)
{
    float params1;
    float params2;
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
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 1.0f;
    
    float4 shadow = shadowTexture.Sample(textureSampler, input.texCoord);
    
    float2 texCoord = input.texCoord;
    texCoord.x += params1;
    texCoord.y += params2;
    
    if (shadow.r < 0.5f && shadow.r != 0.0f)
    {
        finalColor = crossHatchTexture.Sample(textureSampler, texCoord);
    }
    else
    {
        finalColor = comicTexture.Sample(textureSampler, input.texCoord);
    }
    
     return finalColor;
}
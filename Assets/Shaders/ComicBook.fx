//Comic Book effect for rendering objects

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

float4 PS(VS_OUTPUT input) : SV_Target
{
    float w = 1.0f / screenWidth;
    float h = 1.0f / screenHeight;
    
    float4 n[9];
    n[0] = textureMap.Sample(textureSampler, input.texCoord + float2(  -w,   -h));
    n[1] = textureMap.Sample(textureSampler, input.texCoord + float2(0.0f,   -h));
    n[2] = textureMap.Sample(textureSampler, input.texCoord + float2(   w,   -h));
    n[3] = textureMap.Sample(textureSampler, input.texCoord + float2(  -w, 0.0f));
    n[4] = textureMap.Sample(textureSampler, input.texCoord);
    n[5] = textureMap.Sample(textureSampler, input.texCoord + float2(   w, 0.0f));
    n[6] = textureMap.Sample(textureSampler, input.texCoord + float2(  -w,    h));
    n[7] = textureMap.Sample(textureSampler, input.texCoord + float2(0.0f,    h));
    n[8] = textureMap.Sample(textureSampler, input.texCoord + float2(   w,    h));
    
    float4 sobelEdge_h = n[2] + (2.0f * n[5]) + n[8] - (n[0] + (2.0f * n[3]) + n[6]);
    float4 sobelEdge_v = n[0] + (2.0f * n[1]) + n[2] - (n[6] + (2.0f * n[7]) + n[8]);
    float4 sobel = sqrt((sobelEdge_h * sobelEdge_h) + (sobelEdge_v * sobelEdge_v));
   
    return float4(1.0f - sobel.rgb, 1.0f);
}
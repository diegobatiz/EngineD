//Comic Book effect for rendering objects

cbuffer SettingsBuffer : register(b0)
{
    float screenWidth;
    float screenHeight;
    float multiplier;
}

Texture2D normalTexture : register(t0);
Texture2D depthTexture : register(t1);

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
    float4 finalColor = 1.0f;
    float w = 1.0f / screenWidth;
    float h = 1.0f / screenHeight;
    
    float4 n[9];
    n[0] = normalTexture.Sample(textureSampler, input.texCoord + float2(-w, -h));
    n[1] = normalTexture.Sample(textureSampler, input.texCoord + float2(0.0f, -h));
    n[2] = normalTexture.Sample(textureSampler, input.texCoord + float2(w, -h));
    n[3] = normalTexture.Sample(textureSampler, input.texCoord + float2(-w, 0.0f));
    n[4] = normalTexture.Sample(textureSampler, input.texCoord);
    n[5] = normalTexture.Sample(textureSampler, input.texCoord + float2(w, 0.0f));
    n[6] = normalTexture.Sample(textureSampler, input.texCoord + float2(-w, h));
    n[7] = normalTexture.Sample(textureSampler, input.texCoord + float2(0.0f, h));
    n[8] = normalTexture.Sample(textureSampler, input.texCoord + float2(w, h));
    
    float4 sobelEdge_h1 = n[2] + (2.0f * n[5]) + n[8] - (n[0] + (2.0f * n[3]) + n[6]);
    float4 sobelEdge_v1 = n[0] + (2.0f * n[1]) + n[2] - (n[6] + (2.0f * n[7]) + n[8]);
    float4 sobel1 = sqrt((sobelEdge_h1 * sobelEdge_h1) + (sobelEdge_v1 * sobelEdge_v1));
    
    n[0] = depthTexture.Sample(textureSampler, input.texCoord + float2(-w, -h));
    n[1] = depthTexture.Sample(textureSampler, input.texCoord + float2(0.0f, -h));
    n[2] = depthTexture.Sample(textureSampler, input.texCoord + float2(w, -h));
    n[3] = depthTexture.Sample(textureSampler, input.texCoord + float2(-w, 0.0f));
    n[4] = depthTexture.Sample(textureSampler, input.texCoord);
    n[5] = depthTexture.Sample(textureSampler, input.texCoord + float2(w, 0.0f));
    n[6] = depthTexture.Sample(textureSampler, input.texCoord + float2(-w, h));
    n[7] = depthTexture.Sample(textureSampler, input.texCoord + float2(0.0f, h));
    n[8] = depthTexture.Sample(textureSampler, input.texCoord + float2(w, h));
    
    float4 sobelEdge_h2 = n[2] + (2.0f * n[5]) + n[8] - (n[0] + (2.0f * n[3]) + n[6]);
    float4 sobelEdge_v2 = n[0] + (2.0f * n[1]) + n[2] - (n[6] + (2.0f * n[7]) + n[8]);
    float4 sobel2 = sqrt((sobelEdge_h2 * sobelEdge_h2) + (sobelEdge_v2 * sobelEdge_v2));
   
    if (sobel1.r + sobel1.g + sobel1.b > 1.5f)
    {
        finalColor = float4(0, 0, 0, 0);
    }
    
    if (sobel2.r + sobel2.g + sobel2.b > 1.5f)
    {
        finalColor = float4(0, 0, 0, 0);
    }
    
    return finalColor;
}
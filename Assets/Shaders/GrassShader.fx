//simple shader that only uses position and color and applies transform

cbuffer WorldBuffer : register(b0)
{
    matrix wvp;
}

cbuffer ColorBuffer : register(b1)
{
    float4 albedo1Color;
    float4 albedo2Color;
    float4 AOColor;
    float4 tipColor;
}

Texture2D grassTexture : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3 id : INSTANCEPOS;
    float height : HEIGHT;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};




VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    
    float extraHeight = (input.height + 2.0f) * 1.5f;
    input.position.y += (input.texCoord.y - 1.0f) * -1.0f * extraHeight;
    
    input.position.xz += input.id.xz; 
    
    output.position = mul(float4(input.position, 1.0f), wvp);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = lerp(albedo1Color, albedo2Color, input.texCoord.y);
    
    float4 ao = lerp(1.0f, AOColor, input.texCoord.y);
    float4 tip = lerp(tipColor, 0.0f, input.texCoord.y * input.texCoord.y);
    
    float4 finalColour = (color + tip) * ao;

    return finalColour;
}


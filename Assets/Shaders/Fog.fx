//Description: Fog Effect

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    float3 cameraPos;
}

cbuffer ColorBuffer : register(b1)
{
    float4 fogColour;
    float fogDensity;
    float fogOffset;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 worldPos : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.worldPos = input.position.xz;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.color = input.color;;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float distance = length(cameraPos.xz - input.worldPos);
    float fogFactor = (fogDensity / sqrt(log(2))) * (max(0.0f, distance - 0.0f));
    fogFactor = exp2(-fogFactor * fogFactor);
    
    return lerp(fogColour, input.color, fogFactor);
}

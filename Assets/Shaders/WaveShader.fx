//simple shader that only uses position and color and applies transform

cbuffer WorldBuffer : register(b0)
{
    matrix wvp;
    matrix worldMatrix;
}

cbuffer TimeBuffer : register(b1)
{
    float time;
}

cbuffer WaveBuffer : register(b2)
{
    int waveCount;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
    float4 color : COLOR;
    float3 normal : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
};

struct Wave
{
    float2 direction;
    float2 origin;
    float frequency;
    float amplitude;
    float phase;
    float steepness;
};

StructuredBuffer<Wave> waves : register(t0);

float2 GetDirection(float3 pos, Wave wave)
{
    return wave.direction;
}

float GetWaveCoord(float3 pos, float2 dir, Wave wave)
{
    return pos.x * dir.x + pos.z * dir.y;
}

float GetTime(Wave wave)
{
    return time * wave.phase;
}

float Sine(float3 worldPos, Wave wave)
{
    float2 dir = GetDirection(worldPos, wave);
    float pos = GetWaveCoord(worldPos, dir, wave);
    float t = GetTime(wave);

    return wave.amplitude * sin(pos * wave.frequency + t);
}

float3 CalculateOffset(float3 worldPos, Wave wave)
{
    return 1.0f;
}


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    
    output.worldPos = mul(float4(input.position, 1.0f), worldMatrix);
    
    float height = 0.0f;
    
    for (int wi = 0; wi < waveCount; ++wi)
    {
        height += CalculateOffset(output.worldPos, waves[wi]);
    }
    
    float4 newPos = float4(input.position, 1.0f);
    newPos.y += height;
    
    output.worldPos = mul(newPos, worldMatrix);
    output.position = mul(newPos, wvp);
    output.color = input.color;
    output.normal = 0.0f;
	
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;

}


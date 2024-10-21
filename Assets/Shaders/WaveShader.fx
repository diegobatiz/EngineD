//simple shader that only uses position and color and applies transform

cbuffer WorldBuffer : register(b0)
{
    matrix wvp;
    matrix worldMatrix;
    float3 lightDirection;
    float3 cameraPos;
}

cbuffer SettingsBuffer : register(b1)
{
    float normalStrength;
    float3 diffuseReflectance;
}

cbuffer TimeBuffer : register(b2)
{
    float time;
}

cbuffer WaveBuffer : register(b3)
{
    int waveCount;
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

#define PI 3.1415926f

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

float3 SineNormal(float3 worldPos, Wave wave)
{
    float2 dir = GetDirection(worldPos, wave);
    float pos = GetWaveCoord(worldPos, dir, wave);
    float t = GetTime(wave);

    float2 n = wave.frequency * wave.amplitude * dir * cos(pos * wave.frequency + t);

    return float3(-n.x, 1.0f, -n.y);
}

float CalculateOffset(float3 worldPos, Wave wave)
{
    return Sine(worldPos, wave);
}

float3 CalculateNormal(float3 worldPos, Wave wave)
{
    return SineNormal(worldPos, wave);
}


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    
    float height = 0.0f;
    float3 normal = 0.0f;
    
   
    
    for (int wi = 0; wi < waveCount; ++wi)
    {
       height += CalculateOffset(input.position, waves[wi]);
       normal += CalculateNormal(input.position, waves[wi]);
    }
    
    output.worldPos = input.position;
    
    input.position.y += height;
   
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldPos = mul(input.position, (float3x3) worldMatrix);
    output.color = input.color;
    output.normal = float3(0.0f, 0.0f, 0.0f);
	
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 lightDir = -normalize(lightDirection);
    //float3 viewDir = normalize(cameraPos - input.worldPos);
    //float3 halfwayDir = normalize(lightDir + viewDir);
    
    float3 normal = 0.0f;
    
    for (int wi = 0; wi < 1; ++wi)
    {
        normal += CalculateNormal(input.worldPos, waves[wi]);
    }
    
    normal = normalize(normal);
    normal = normalize(mul(normal, (float3x3) worldMatrix));
    
    normal.xz *= normalStrength;
    normal = normalize(normal);
    
    float ndotl = max(0.0f, dot(lightDir, normal));
    
    float3 diffReflect = diffuseReflectance / 3.1415926f;
    float3 diffuse = input.color.rgb * ndotl * diffReflect;
    
    return float4(diffuse, 1.0f);
}


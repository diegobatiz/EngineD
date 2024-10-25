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
    float specNormalStrength;
    float3 specularReflectance;
    float shininess;
    float3 ambientColor;
    float4 specColour;
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

float SteepSine(float3 worldPos, Wave wave)
{
    float2 dir = GetDirection(worldPos, wave);
    float pos = GetWaveCoord(worldPos, dir, wave);
    float t = GetTime(wave);

    return 2.0f * wave.amplitude * pow((sin(pos * wave.frequency + t) + 1.0f) * 0.5f, wave.steepness);
}

float3 SteepSineNormal(float3 worldPos, Wave wave)
{
    float2 dir = GetDirection(worldPos, wave);
    float pos = GetWaveCoord(worldPos, dir, wave);
    float t = GetTime(wave);
    
    float h = pow((sin(pos * wave.frequency + t) + 1) * 0.5f, max(1.0f, wave.steepness - 1));
    float2 n = dir * wave.steepness * wave.frequency * wave.amplitude * h * cos(pos * wave.frequency + t);

    return float3(-n.x, 1.0f, -n.y);
}

//Fractional Brownian Motion
float3 VertexFBM(float3 pos)
{
    float f = _VertexFrequency;
    float a = _VertexAmplitude;
    float speed = _VertexInitialSpeed;
    float seed = _VertexSeed;
    float3 p = v;
    float amplitudeSum = 0.0f;

    float h = 0.0f;
    float2 n = 0.0f;
    for (int wi = 0; wi < waveCount; ++wi)
    {
        float2 d = normalize(float2(cos(seed), sin(seed)));

        float x = dot(d, p.xz) * f + time * speed;
        float wave = a * exp(_VertexMaxPeak * sin(x) - _VertexPeakOffset);
        float dx = _VertexMaxPeak * wave * cos(x);
					
        h += wave;
					
        p.xz += d * -dx * a * _VertexDrag;

        amplitudeSum += a;
        f *= _VertexFrequencyMult;
        a *= _VertexAmplitudeMult;
        speed *= _VertexSpeedRamp;
        seed += _VertexSeedIter;
    }

    float3 output = float3(h, n.x, n.y) / amplitudeSum;
    output.x *= _VertexHeight;

    return output;
}

float CalculateOffset(float3 worldPos, Wave wave)
{
    return SteepSine(worldPos, wave);
}

float3 CalculateNormal(float3 worldPos, Wave wave)
{
    return SteepSineNormal(worldPos, wave);
}


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    
    float height = 0.0f;
    float3 normal = 0.0f;
    
    for (int wi = 0; wi < waveCount; ++wi)
    {
       height += CalculateOffset(input.position, waves[wi]);
    }
    
    output.worldPos = input.position;
    
    input.position.y += height;
   
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldPos = mul(input.position, (float3x3) worldMatrix);
    output.color = input.color;
    output.normal = 0.0f;
	
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 lightDir = -normalize(lightDirection);
    float3 viewDir = normalize(cameraPos - input.worldPos);
    float3 halfwayDir = normalize(lightDir + viewDir);
    
    //Pixel Normal
    float3 normal = 0.0f;
    
    for (int wi = 0; wi < waveCount; ++wi)
    {
        normal += CalculateNormal(input.worldPos, waves[wi]);
    }
    normal = normalize(normal);
    normal = normalize(mul(normal, (float3x3) worldMatrix));
    normal.xz *= normalStrength;
    normal = normalize(normal);
    
    float ndotl = max(0.0f, dot(lightDir, normal));
    
    //Diffuse Colour
    float3 diffReflect = diffuseReflectance / PI;
    float3 diffuse = input.color.rgb * ndotl * diffReflect;
    
    //Specular Colour
    float3 specReflect = specularReflectance;
    float3 specNormal = normal;
    specNormal.xz *= specNormalStrength;
    specNormal = normalize(specNormal);
    float spec = pow(max(0.0f, dot(specNormal, halfwayDir)), shininess) * ndotl;
    float3 specular = specColour.rgb * specReflect * spec;
    
    float3 finalColor = diffuse +  ambientColor + specular;
    
    return float4(finalColor, 1.0f);
}


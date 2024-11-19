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
    float tipAttenuation;
    float3 tipColor;
    float4 diffuseColor;
    float fresnelNormalStrength;
    float fresnelShininess;
    float fresnelBias;
    float fresnelStrength;
    float4 fresnelColor;
}

cbuffer TimeBuffer : register(b2)
{
    float time;
}

cbuffer WaveBuffer : register(b3)
{
    int waveCount;
    int pixelWaveCount;
    float vertexFrequency;
    float vertexAmplitude;
    float vertexInitialSpeed;
    float vertexSeed;
    float vertexMaxPeak;
    float vertexPeakOffset;
    float vertexFrequencyMult;
    float vertexAmplitudeMult;
    float vertexSpeedRamp;
    float vertexSeedIter;
    float vertexHeight;
    float vertexDrag;
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

//Used in Sum of Sines version of Shader
struct Wave
{
    float2 direction;
    float2 origin;
    float frequency;
    float amplitude;
    float phase;
    float steepness;
};

//Used in Sum of Sines version of Shader
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

float CalculateOffset(float3 worldPos, Wave wave)
{
    return SteepSine(worldPos, wave);
}

float3 CalculateNormal(float3 worldPos, Wave wave)
{
    return SteepSineNormal(worldPos, wave);
}


//Fractional Brownian Motion
float3 VertexFBM(float3 pos)
{
    float f = vertexFrequency;
    float a = vertexAmplitude;
    float speed = vertexInitialSpeed;
    float seed = vertexSeed;
    float3 p = pos;
    float amplitudeSum = 0.0f;

    float h = 0.0f;
    float2 n = 0.0f;
    for (int wi = 0; wi < 32; ++wi)
    {
        float2 d = normalize(float2(cos(seed), sin(seed)));

        float x = dot(d, p.xz) * f + time * speed;
        float wave = a * exp(vertexMaxPeak * sin(x) - vertexPeakOffset);
        float dx = vertexMaxPeak * wave * cos(x);
					
        h += wave;
					
        p.xz += d * -dx * a * vertexDrag;

        amplitudeSum += a;
        f *= vertexFrequencyMult;
        a *= vertexAmplitudeMult;
        speed *= vertexSpeedRamp;
        seed += vertexSeedIter;
    }

    float3 output = float3(h, n.x, n.y) / amplitudeSum;
    output.x *= 1.0f;

    return output;
}

float3 PixelFBM(float3 pos)
{
    float f = vertexFrequency;
    float a = vertexAmplitude;
    float speed = vertexInitialSpeed;
    float seed = vertexSeed;
    float3 p = pos;

    float h = 0.0f;
    float2 n = 0.0f;
				
    float amplitudeSum = 0.0f;

    for (int wi = 0; wi < 32; ++wi)
    {
        float2 d = normalize(float2(cos(seed), sin(seed)));

        float x = dot(d, p.xz) * f + time * speed;
        float wave = a * exp(vertexMaxPeak * sin(x) - vertexPeakOffset);
        float2 dw = f * d * (vertexMaxPeak * wave * cos(x));
					
        h += wave;
        p.xz += -dw * a * vertexDrag;
					
        n += dw;
					
        amplitudeSum += a;
        f *= vertexFrequencyMult;
        a *= vertexAmplitudeMult;
        speed *= vertexSpeedRamp;
        seed += vertexSeedIter;
    }
				
    float3 output = float3(h, n.x, n.y) / amplitudeSum;
    output.x *= vertexHeight;

    return output;
}


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    
    float height = 0.0f;
    float3 normal = 0.0f;
   
    float3 vertexOutput = VertexFBM(input.position);
    
    output.worldPos = input.position;
    
    input.position.y += vertexOutput.x;
   
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
    float height = 0.0f;
    
    float3 pixelOutput = PixelFBM(input.worldPos);
    
    height = pixelOutput.x;
    normal.xy = pixelOutput.yz;
    
    normal = normalize(float3(-normal.x, 1.0f, -normal.y));
    normal = normalize(mul(normal, (float3x3) worldMatrix));
    normal.xz *= normalStrength;
    normal = normalize(normal);
    
    float ndotl = max(0.0f, dot(lightDir, normal));
    
    //Diffuse Colour
    float3 diffReflect = diffuseReflectance / PI;
    float3 diffuse = diffuseColor.rgb * ndotl * diffReflect;
    
    // Schlick Fresnel
    float3 fresnelNormal = normal;
    fresnelNormal.xz *= fresnelNormalStrength;
    fresnelNormal = normalize(fresnelNormal);
    float base = 1 - dot(viewDir, fresnelNormal);
    float exponential = pow(base, fresnelShininess);
    float R = exponential + fresnelBias * (1.0f - exponential);
    R *= fresnelStrength;
				
    float3 fresnel = fresnelColor.xyz * R;
    
    //Specular Colour
    float3 specReflect = specularReflectance;
    float3 specNormal = normal;
    specNormal.xz *= specNormalStrength;
    specNormal = normalize(specNormal);
    float spec = pow(max(0.0f, dot(specNormal, halfwayDir)), shininess) * ndotl;
    float3 specular = specColour.xyz * specReflect * spec;
    
    // Schlick Fresnel but again for specular
    base = 1 - max(0.0f, dot(viewDir, halfwayDir));
    exponential = pow(base, 5.0f);
    R = exponential + fresnelBias * (1.0f - exponential);

    specular *= R;
    
    float3 tip = tipColor * pow(height, tipAttenuation);
    
    float3 finalColor = diffuse +  ambientColor + specular + tip + fresnel;
    
    return float4(finalColor, 1.0f);
}


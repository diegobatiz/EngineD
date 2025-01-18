//Description: Position Map -> maps out the player's position

cbuffer PlayerPositionBuffer : register(b0)
{
    float2 playerPosition; //player position in texture space
    float playerRadius;
}

cbuffer TrailBuffer : register(b1)
{
    float snowPower;
    float minStartGradient;
    float maxStartGradient;
    float snowFill;
    float reset;
}

cbuffer TimeBuffer : register(b2)
{
    float time;
    float cycleDuration;
}

Texture2D snowHeightMap : register(t0);
SamplerState texSampler : register(s0);

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
    output.position = float4(input.position, 1.0);
    output.texCoord = input.texCoord;
    return output;
}

float hash(float2 uv)
{
    return frac(sin(7.289 * uv.x + 11.23 * uv.y) * 23758.5453);
}

float smoothInterpolate(float current, float next, float t)
{
    float alpha = t * t * t * (t * (t * 6.0 - 15.0) + 10);
    return lerp(current, next, alpha);
}

float randomInterpolate(float min, float max, float cycleTime, float t)
{
    float cycle = floor(t / cycleTime);
    float currentTarget = min + (max - min) * hash(float2(cycle, 0.0));
    float nextTarget = min + (max - min) * hash(float2(cycle + 1, 0.0));
    
    float tea = fmod(t, cycleTime) / cycleTime;
    
    return smoothInterpolate(currentTarget, nextTarget, tea);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float distance = length(input.texCoord - playerPosition);
   
    float startDisplace = randomInterpolate(minStartGradient, maxStartGradient, cycleDuration, time);
    
    float distanceFromCenter = clamp(distance / playerRadius, 0, 10);
    
    float snowDisplacement = 1.0;
    if (distanceFromCenter > startDisplace)
    {
        snowDisplacement = 1.0 - pow(abs(distanceFromCenter - startDisplace), snowPower);
        clamp(snowDisplacement, 0, 1);
    }
    
    float currentHeight = snowHeightMap.Sample(texSampler, input.texCoord).r;
    
    float deeper = max(snowDisplacement, currentHeight);
    
    if (deeper == currentHeight && deeper > 0.37)
    {
        deeper -= 0.0005 * snowFill;
        deeper = max(deeper, 0.0);
    }
    
    return float4(deeper * reset, 0.0, 0.0, 1.0);
}

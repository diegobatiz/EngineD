//Description: Fog Effect

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    float3 cameraWorldPos;
}

cbuffer TessellationBuffer : register(b1)
{
    float minTessDistance;
    float maxTessDistance;
    float tessLevel;
}

Texture2D positionMap : register(t0);

SamplerState texSampler : register(s0);

//=================//Vertex Shader//====================//
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position;
    output.color = input.color;
    output.texCoord = input.texCoord;
    return output;
}

//=================//Hull Shader//====================//
struct HS_OUTPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

struct PatchConstantData
{
    float edgeTess[3];
    float insideTess;
};

float CalculateTessFactor(float3 worldPosition, float minDist, float maxDist, float level)
{
    float d = distance(worldPosition, cameraWorldPos);
    float f = clamp(1.0 - (d - minDist) / (maxDist - minDist), 0.01, 1.0);

    return f * level;
}

float4 DistanceBasedTess(float3 v0, float3 v1, float3 v2, float minDist, float maxDist, float level)
{
    float3 f;
    f.x = CalculateTessFactor(v0, minDist, maxDist, level);
    f.y = CalculateTessFactor(v1, minDist, maxDist, level);
    f.z = CalculateTessFactor(v2, minDist, maxDist, level);
    
    float4 tess;
    tess.x = 0.5 * (f.y + f.z);
    tess.y = 0.5 * (f.x + f.z);
    tess.z = 0.5 * (f.x + f.y);
    tess.w = (f.x + f.y + f.z) / 3.0;
    
    return tess;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[patchconstantfunc("PatchConstantFunction")]
[outputcontrolpoints(3)]
HS_OUTPUT HSControlPoint(InputPatch<VS_OUTPUT, 3> inputPatch, uint controlPointId : SV_OutputControlPointID)
{
    HS_OUTPUT output;
    
    output.position = inputPatch[controlPointId].position;
    output.texCoord = inputPatch[controlPointId].texCoord;
    output.color = inputPatch[controlPointId].color;
    
    return output;
}

PatchConstantData PatchConstantFunction(InputPatch<VS_OUTPUT, 3> inputPatch)
{
    PatchConstantData patchData;
    
    float4 tessFactor = DistanceBasedTess(inputPatch[0].position, inputPatch[1].position, inputPatch[2].position, minTessDistance, maxTessDistance, tessLevel);
    
    patchData.edgeTess[0] = tessFactor.x;
    patchData.edgeTess[1] = tessFactor.y;
    patchData.edgeTess[2] = tessFactor.z;
    patchData.insideTess = tessFactor.w;
    
    return patchData;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 displacement = positionMap.Sample(texSampler, input.texCoord);
    
    if (displacement.r != 0.0)
    {
        return float4(0.0, 0.0, 0.0, 0.0);
    }
    
    return input.color;
}

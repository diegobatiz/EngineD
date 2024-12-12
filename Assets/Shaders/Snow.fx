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
    float bumpOffset;
}

cbuffer LightingSettings : register(b2)
{
    float4 bottomColor;
    float4 topColor;
};

Texture2D positionMap : register(t0);
Texture2D snowBumpMap : register(t1);
Texture2D snowTexture : register(t2);

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
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
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




//=================//Domain Shader//====================//

struct DS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float height : TEXCOORD1;
};

[domain("tri")]
DS_OUTPUT DS(PatchConstantData patchConstants, float3 coords : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
    DS_OUTPUT output;
    
    float3 position =
        coords.x * patch[0].position +
        coords.y * patch[1].position +
        coords.z * patch[2].position;
    
    float2 texCoord =
        patch[0].texCoord * coords.x +
        patch[1].texCoord * coords.y +
        patch[2].texCoord * coords.z;
    
    float4 color =
         patch[0].color * coords.x +
         patch[1].color * coords.y +
         patch[2].color * coords.z;
    
    float height = positionMap.SampleLevel(texSampler, texCoord, 0).r;
	float useBump = step(height, 0.9);
    float bump = snowBumpMap.SampleLevel(texSampler, texCoord, 0).r * useBump;
    position.y = -height + (bump - (0.85 * useBump));
    
    output.height = position.y;
    
    position.y += (useBump * bumpOffset);

    output.position = mul(float4(position, 1.0), wvp);
    output.texCoord = texCoord;
    output.color = color;
    
    return output;
}


//=================//Pixel Shader//====================//
/*
float3 ComputeNormalFromHeightMap(float2 texCoord)
{
    float2 texelSize = 1.0 / texSize;
    
    float heightCenter = positionMap.Sample(texSampler, texCoord).r;
    float heightLeft = positionMap.Sample(texSampler, texCoord - float2(texelSize.x, 0)).r;
    float heightUp = positionMap.Sample(texSampler, texCoord - float2(0, texelSize.y)).r;
    
    float dX = (heightCenter - heightLeft) * texSize;
    float dY = (heightCenter - heightUp) * texSize;

    float3 normal = normalize(float3(-dX * normalStrength, 1.0, -dY * normalStrength));

    return normal;
}
*/

float4 PS(DS_OUTPUT input) : SV_Target
{
    float4 top = snowTexture.Sample(texSampler, input.texCoord);
    float4 color = lerp(bottomColor, topColor, input.height + 1);
    
    return color;
}

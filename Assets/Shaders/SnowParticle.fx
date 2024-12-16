//render a particle

cbuffer ParticleBuffer : register(b0)
{
    matrix matView;
    matrix matProj;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float4 id : INSTANCEPOS;
    float2 noise : NOISE;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
};

#define MATRIX_IDENTITY = 

float3 TransformCoord(float3 position, matrix m)
{
    float3 output;
    output.x = position.x * m._11 + position.y * m._12 + position.z * m._13 + m._14;
    output.y = position.x * m._21 + position.y * m._22 + position.z * m._23 + m._24;
    output.z = position.x * m._31 + position.y * m._32 + position.z * m._33 + m._34;
    
    return output;
}

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float3 position = float3(input.id.x, input.id.y, input.id.z);
    
    float3 viewPos = TransformCoord(position, matView);
    
    float3 worldPos = input.position + viewPos;
    
    output.position = mul(matProj, float4(worldPos, 1.0));
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1.0, 1.0, 1.0, 1.0);
}
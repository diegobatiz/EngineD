//simple shader that only uses position and color and applies transform

cbuffer WorldBuffer : register(b0)
{
    matrix wvp;
}

cbuffer TimeBuffer : register(b1)
{
    float time;
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
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.color = input.color;
	
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;

}


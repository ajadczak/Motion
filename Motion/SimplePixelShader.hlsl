cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};

cbuffer cbPerFrame : register(b1)
{
	float3 gLightDirection;
	float3 gLightPosition;
	float4 gLightColor;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(VertexOut vin) : SV_TARGET
{
	return vin.Color;
}
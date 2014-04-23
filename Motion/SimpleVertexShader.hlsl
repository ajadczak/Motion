//#pragma pack_matrix(row_major)

cbuffer cbPerFrame : register(b0)
{
	float3 gLightDirection;
	float3 gLightPosition;
	float4 gLightColor;

	/*
	float4x4 gView;
	float4x4 gInvView;
	float4x4 gProj;
	float4x4 gInvProj;
	float4x4 gViewProj;

	DirectionalLight gDirLights[3];
	float3 gEyePosW;
	float perFramePad0;

	float4 gFogColor;
	float gFogStart;
	float gFogRange;
	*/
};

cbuffer cbPerObject : register(b1)
{
	float4x4 gWorldViewProj;

	/*
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gTexTransform;
	Material gMaterial;
	*/
};

struct VertexIn
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut main(VertexIn vin) 
{
	//transform to homogenous clip space 
	VertexOut vout;
	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	vout.Color = vin.Color;
	return vout;
}
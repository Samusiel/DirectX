struct IA2VS_ModelData
{
	float4 pos : POSITION;
	float2 tc  : TEXCOORD0;
};

struct VS2PS_ModelData
{
	float4 pos : SV_POSITION;
	float2 tc  : TEXCOORD0;
};

cbuffer MatrixBuffer : register(c0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix viewProjectionMatrix;
};
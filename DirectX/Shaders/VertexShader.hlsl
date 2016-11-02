#include "types_definitions.hlsli"

VS2PS_ModelData main( IA2VS_ModelData i)
{
	VS2PS_ModelData output = (VS2PS_ModelData)0;
	output.pos = float4(i.pos.xyz, 1);

	output.tc = i.tc;
	return output;
}
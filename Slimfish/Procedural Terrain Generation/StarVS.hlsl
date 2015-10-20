

struct VSOutput
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;
};

VSOutput main( float4 pos : POSITION )
{
	VSOutput vOut;
	vOut.pos = pos;
	vOut.posW = pos.xyz;
	return vOut;
}

struct VSInput {
	float3 position : POSITION;
};

struct VSOutput {
	float4 position : SV_POSITION;
};

VSOutput main( VSInput vIn )
{
	VSOutput vOut;

	vOut.position = float4(vIn.position.xyz, 1.0f);

	return vOut;
}
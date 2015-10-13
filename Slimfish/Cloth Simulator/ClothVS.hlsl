cbuffer constantBuffer {
	float4x4 gWVP;
};

struct VSInput {
	float3 position : POSTION;
	float3 normal : NORMAL;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

VSOutput main( VSInput vIn )
{
	VSOutput vOut;

	vOut.pos = mul(float4(vIn.position, 1.0f), gWVP);
	vOut.normal = vIn.normal;

	return vOut;
}
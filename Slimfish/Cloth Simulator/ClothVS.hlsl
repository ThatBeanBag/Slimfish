cbuffer constantBuffer {
	float4x4 gWVP;
};

struct VSInput {
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

VSOutput main( VSInput vIn )
{
	VSOutput vOut;

	vOut.position = mul(float4(vIn.position, 1.0f), gWVP);
	vOut.normal = vIn.normal;

	return vOut;
}
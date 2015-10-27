
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
	// vOut.wPosition = float4(vIn.position.xyz, 1.0f);
	// vOut.position = mul(float4(vIn.position.xyz, 1.0f), gWorldViewProjectionMatrix);
	// vOut.colour = float4(1.0f, 0.0f, 0.0f, 1.0f);

	return vOut;
}
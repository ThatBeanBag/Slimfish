cbuffer constantBuffer {
	float4x4 gWorldViewProjMatrix;
	float4x4 gLightViewProjMatrix;
};

struct VSInput {
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 lightViewProjPosition : TEXCOORD0;
};

VSOutput main( VSInput vIn )
{
	VSOutput vOut;

	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProjMatrix);
	vOut.normal = vIn.normal;
	vOut.lightViewProjPosition = mul(float4(vIn.position, 1.0f), gLightViewProjMatrix);

	return vOut;
}
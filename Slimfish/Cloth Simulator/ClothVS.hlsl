cbuffer constantBuffer {
	float4x4 gWorldMatrix;
	float4x4 gWorldViewProjMatrix;
	float4x4 gLightViewProjMatrix;
};

struct VSInput {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 colour : COLOR;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float4 lightViewProjPosition : TEXCOORD1;
	float4 colour : COLOR;
};

VSOutput main( VSInput vIn )
{
	VSOutput vOut;

	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProjMatrix);
	vOut.normal = mul(float4(vIn.normal, 0.0f), gWorld);
	vOut.lightViewProjPosition = mul(float4(vIn.position, 1.0f), gLightViewProjMatrix);
	vOut.colour = vIn.colour;
	vOut.texCoord = vIn.texCoord;

	return vOut;
}
cbuffer constantBuffer {
	float4x4 gWorldMatrix;
	float4x4 gWorldViewProjMatrix;
	float4x4 gLightViewProjMatrix;
	float3 gEyePosition;
};

struct VSInput {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 colour : COLOR;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float4 positionW : TEXCOORD0;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD1;
	float4 lightViewProjPosition : TEXCOORD2;
	float4 colour : COLOR;
};

VSOutput main( VSInput vIn )
{
	VSOutput vOut;

	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProjMatrix);
	vOut.positionW = mul(float4(vIn.position, 1.0f), gWorldMatrix);
	vOut.normal = mul(float4(vIn.normal, 0.0f), gWorldMatrix);
	vOut.lightViewProjPosition = mul(float4(vIn.position, 1.0f), gLightViewProjMatrix);
	vOut.colour = vIn.colour;
	vOut.texCoord = vIn.texCoord;

	return vOut;
}
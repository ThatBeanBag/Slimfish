
cbuffer constantBuffer {
	float4x4 gWorldMatrix;
	float4x4 gLightViewMatrix;
	float4x4 gLightProjectionMatrix;
};

struct VS_IN {
	float3 position : POSITION;
};

struct PS_INPUT {
	float4 position : SV_POSITION;
	float depth : TEXCOORD0;
};

PS_INPUT main(VS_IN vIn)
{
	PS_INPUT vOut;

	float4x4 lightViewProj = mul(mul(gWorldMatrix, gLightViewMatrix), gLightProjectionMatrix);
	vOut.position = mul(float4(vIn.position, 1.0f), lightViewProj);
	vOut.depth = vOut.position.z / vOut.position.w;

	return vOut;
}
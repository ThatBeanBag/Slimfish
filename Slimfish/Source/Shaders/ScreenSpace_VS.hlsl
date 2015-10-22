
cbuffer constantBuffer {
	float4x4 gWorldMatrix;
	float4x4 gProjectionMatrix;
};

struct VS_IN {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct PS_INPUT {
	float4 positionScreen : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

PS_INPUT main(VS_IN vIn)
{
	PS_INPUT vOut;

	float4x4 worldViewProj = mul(gWorldMatrix, gProjectionMatrix);
	vOut.positionScreen = mul(float4(vIn.position, 1.0f), worldViewProj);
	vOut.texCoord = vIn.texCoord;

	return vOut;
}
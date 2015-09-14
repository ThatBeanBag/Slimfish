

cbuffer cbPerObject {
	float4x4 gWorldMatrix;
	float4x4 gTexMatrix;
};

cbuffer cbPerFrame {
	float4x4 gViewMatrix;
	float4x4 gProjectionMatrix;
};

struct VS_IN {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 tangent : TANGENT;
};

struct VS_OUT {
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 tangent : TANGENT;
};

VS_OUT main(VS_IN vIn)
{
	VS_OUT vOut;

	float4x4 worldViewProj = mul(mul(gWorldMatrix, gViewMatrix), gProjectionMatrix);

	vOut.positionScreen = mul(float4(vIn.position, 1.0f), worldViewProj);
	vOut.positionWorld = mul(float4(vIn.position, 1.0f), gWorldMatrix).xyz;
	vOut.normal = mul(float4(vIn.normal, 0.0f), gWorldMatrix).xyz;
	vOut.tangent = mul(float4(vIn.tangent, 0.0f) gWorldMatrix).xyz;

	vOut.texCoord = mul(float4(vIn.texCoord, 0.0f, 1.0f), gTexMatrix).xy;

	return vOut;
}
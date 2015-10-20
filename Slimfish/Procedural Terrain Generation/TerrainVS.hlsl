
static const float PI = 3.14159265f;

cbuffer constantBuffer {
	float4x4 gWorldMatrix;
	float4x4 gTexMatrix;
	float4x4 gViewMatrix;
	float4x4 gProjectionMatrix;
};

struct VS_IN {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT {
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float4 positionDepth : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD1;
};

VS_OUT main(VS_IN vIn)
{
	VS_OUT vOut;

	float4x4 worldViewProj = mul(mul(gWorldMatrix, gViewMatrix), gProjectionMatrix);

	vOut.positionScreen = mul(float4(vIn.position, 1.0f), worldViewProj);
	vOut.positionWorld = mul(float4(vIn.position, 1.0f), gWorldMatrix).xyz;
	vOut.positionDepth = vOut.positionScreen;
	vOut.normal = mul(float4(vIn.normal, 0.0f), gWorldMatrix).xyz;
	vOut.tangent = mul(float4(vIn.tangent, 0.0f), gWorldMatrix).xyz;
	vOut.texCoord = mul(float4(vIn.texCoord, 0.0f, 1.0f), gTexMatrix).xy;

	return vOut;
}
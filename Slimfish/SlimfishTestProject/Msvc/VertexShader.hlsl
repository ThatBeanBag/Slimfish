
struct Light {
	uint m_Type;
	float4 m_Diffuse;
	float4 m_Specular;
	float3 m_Position;
	float3 m_Direction;
	float m_Range;
	float m_Falloff;
	float3 m_Attenuation;
	float m_SpotTheta;
	float m_SpotPhi;
};

cbuffer cbPerObject {
	float4x4 gWorldMatrix;
	float4x4 gTexMatrix;
};

cbuffer cbPerFrame {
	float4x4 gViewMatrix;
	float4x4 gProjectionMatrix;
};

struct VS_IN
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

VS_OUT main(VS_IN vIn)
{
	VS_OUT vOut;

	float4x4 worldViewProj = mul(mul(gWorldMatrix, gViewMatrix), gProjectionMatrix);

	vOut.positionScreen = mul(float4(vIn.position, 1.0f), worldViewProj);
	vOut.positionWorld = mul(float4(vIn.position, 1.0f), gWorldMatrix).xyz;
	vOut.normal = mul(float4(vIn.normal, 0.0f), gWorldMatrix).xyz;

	vOut.texCoord = mul(float4(vIn.texCoord, 0.0f, 1.0f), gTexMatrix).xy;

	return vOut;
}
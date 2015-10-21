
cbuffer constantBuffer {
	float4x4 gWorldMatrix;
	float4x4 gViewMatrix;
	float4x4 gProjectionMatrix;
	float4 gClipPlane;
};

struct VS_IN {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT {
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
	float clip : SV_ClipDistance0;
};

VS_OUT main(VS_IN vIn)
{
	VS_OUT vOut;

	float4x4 worldViewProj = mul(mul(gWorldMatrix, gViewMatrix), gProjectionMatrix);

	vOut.position = mul(float4(vIn.position, 1.0f), worldViewProj).xyww;	// Return z as 1.
	vOut.texCoord = vIn.position;
	vOut.clip = dot(mul(float4(vIn.position, 1.0f), gWorldMatrix), gClipPlane);

	return vOut;
}
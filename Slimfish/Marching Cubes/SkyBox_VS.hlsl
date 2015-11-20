
cbuffer CBPerFrame {
	float4x4 gWorldViewProjectionMatrix;
};

struct VS_IN {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT {
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

VS_OUT main(VS_IN vIn)
{
	VS_OUT vOut;

	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProjectionMatrix).xyww;	// Return z as 1.
	vOut.texCoord = vIn.position;

	return vOut;
}
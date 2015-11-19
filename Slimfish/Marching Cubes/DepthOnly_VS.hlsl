
cbuffer CBPerFrame {
	float4x4 gWorldViewProjectionMatrix;
	float gZFar;
};

struct VS_IN {
	float3 position : POSITION;
};

struct PS_INPUT {
	float4 position : SV_POSITION;
	float4 positionCoord : TEXCOORD0;
};

PS_INPUT main(VS_IN vIn)
{
	PS_INPUT vOut;

	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProjectionMatrix);
	vOut.positionCoord = vOut.position;
	vOut.positionCoord.z /= vOut.positionCoord.w;
	//vOut.positionCoord.z /= gZFar;

	return vOut;
}
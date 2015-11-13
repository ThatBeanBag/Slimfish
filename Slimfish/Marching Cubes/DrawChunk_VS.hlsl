
struct VSInput
{
	float4 wPositionAmbOcc : POSITION;
	float3 wNormal : NORMAL;
};

struct VSOutput {
	float4 hPosition : SV_POSITION;
	float4 wPosition : TEXCOORD;
	float3 wNormal : NORMAL;
};

cbuffer CBPerFrame {
	float4x4 gViewProjectionMatrix;
};

VSOutput main(VSInput vIn) {
	VSOutput vOut;

	vOut.hPosition = mul(float4(vIn.wPositionAmbOcc.xyz, 1.0f), gViewProjectionMatrix);
	vOut.wPosition = vIn.wPositionAmbOcc.xyzw;
	vOut.wNormal = vIn.wNormal;

	return vOut;
}


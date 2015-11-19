
struct VSInput {
	float4 wPositionAmbOcc : POSITION;
	float3 wNormal : NORMAL;
};

struct VSOutput {
	float4 hPosition : SV_POSITION;
	float3 wNormal : NORMAL;
	float4 wPosition : TEXCOORD0;
	float4 lPosition : TEXCOORD1;
};

cbuffer CBPerFrame {
	float4x4 gViewProjectionMatrix;
	float4x4 gLightViewProjectionMatrix;
};

VSOutput main(VSInput vIn) {
	VSOutput vOut;

	vOut.hPosition = mul(float4(vIn.wPositionAmbOcc.xyz, 1.0f), gViewProjectionMatrix);
	vOut.lPosition = mul(float4(vIn.wPositionAmbOcc.xyz, 1.0f), gLightViewProjectionMatrix);
	vOut.wPosition = vIn.wPositionAmbOcc.xyzw;
	vOut.wNormal = vIn.wNormal;

	return vOut;
}


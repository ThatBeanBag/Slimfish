

struct PS_INPUT {
	float4 position : SV_POSITION;
	float4 positionCoord : TEXCOORD0;
};

float4 main(PS_INPUT pIn) : SV_TARGET
{
	float depth = pIn.positionCoord.z / pIn.positionCoord.w;

	return float4(depth, depth, depth, 1.0f);
}


struct PS_INPUT {
	float4 position : SV_POSITION;
	float depth : TEXCOORD0;
};

float4 main(PS_INPUT pIn) : SV_TARGET
{
	return float4(pIn.depth, pIn.depth, pIn.depth, 1.0f);
}
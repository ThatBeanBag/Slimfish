

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

float4 main(VSOutput pIn) : SV_TARGET
{
	return float(1.0f, 0.0f, 0.0f, 0.0f);
}

TextureCube gDiffuseTexture;
SamplerState gSample0;

struct PS_INPUT {
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
	float clip : SV_ClipDistance0;
};

float4 main(PS_INPUT pIn) : SV_TARGET
{
	return gDiffuseTexture.Sample(gSample0, pIn.texCoord);
}
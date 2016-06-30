
Texture2D gDiffuseTexture;
SamplerState gSample0;

struct PS_INPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float4 colour : COLOUR;
};

float4 main(PS_INPUT pIn) : SV_TARGET
{
	return gDiffuseTexture.Sample(gSample0, pIn.texCoord) * pIn.colour;
}
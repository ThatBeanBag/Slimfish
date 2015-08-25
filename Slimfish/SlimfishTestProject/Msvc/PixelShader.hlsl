
Texture2D gDiffuseTexture;
SamplerState gSample0;

struct PS_INPUT
{
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT pIn) : SV_TARGET
{
	float4 diffuse = gDiffuseTexture.Sample(gSample0, pIn.texCoord);

	return diffuse;
}
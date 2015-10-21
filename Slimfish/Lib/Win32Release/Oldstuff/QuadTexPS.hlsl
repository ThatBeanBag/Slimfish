
Texture2D gDiffuseTexture;
SamplerState gSample0;

struct PS_INPUT {
	float4 positionScreen : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT pIn) : SV_TARGET
{
	float2 texCoords;
	texCoords.x = pIn.positionScreen.x / pIn.positionScreen.w / 2.0f + 0.5f;
	texCoords.y = -pIn.positionScreen.y / pIn.positionScreen.w / 2.0f + 0.5f;

	return gDiffuseTexture.Sample(gSample0, pIn.texCoord);
}
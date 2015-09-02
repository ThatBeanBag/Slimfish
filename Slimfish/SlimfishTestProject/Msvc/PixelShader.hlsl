
#include "LightingHelper.hlsli"

cbuffer cbPerFrame {
	Light gLight;
	float3 gEyePosition;
	float4 gAmbientLight;
};

Texture2D gDiffuseTexture;
Texture2D gSpecularTexture;
SamplerState gSample0;
SamplerState gSample1;

struct PS_INPUT
{
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT pIn) : SV_TARGET
{
	pIn.normal = normalize(pIn.normal);

	float4 diffuse = gDiffuseTexture.Sample(gSample0, pIn.texCoord);
	float4 specular = gSpecularTexture.Sample(gSample1, pIn.texCoord);

	SurfaceInfo v = { pIn.positionScreen.xyz, pIn.normal, diffuse, specular };

	float3 lightColour;
	if (gLight.m_Type == 0) {
		lightColour = ParallelLight(v, gLight, gEyePosition);
	}
	else if (gLight.m_Type == 1) {
		lightColour = PointLight(v, gLight, gEyePosition);
	}
	else {
		lightColour = SpotLight(v, gLight, gEyePosition);
	}

	lightColour += diffuse * gAmbientLight;

	return float4(lightColour, diffuse.a);
}
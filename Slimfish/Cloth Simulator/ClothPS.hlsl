
#include "LightingHelper.hlsli"

cbuffer constantBuffer {
	Light gLight;
	float3 gAmbientLight;
};

static const float bias = 0.1f;

Texture2D gTextureDiffuse;
Texture2D gTextureShadowMap;
SamplerState gSampleDiffuse;
SamplerState gSampleShadowMap;

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float4 lightViewProjPosition : TEXCOORD1;
	float4 colour : COLOR;
};

float4 main(VSOutput pIn) : SV_TARGET
{
	float4 diffuse = gTextureDiffuse.Sample(gSampleDiffuse, texCoord);
	float3 toEye = float3(0.0f, 1.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 133.0f };

	float3 lightColour = float3(0.0f, 0.0f, 0.0f);

	float bCalculateLighting = true;
	// Calculate shadow mapping coords.
	/*float2 lightProjectTexCoords;
	lightProjectTexCoords.x = pIn.lightViewProjPosition.x / pIn.lightViewProjPosition.w / 2.0f + 0.5f;
	lightProjectTexCoords.y = -pIn.lightViewProjPosition.y / pIn.lightViewProjPosition.w / 2.0f + 0.5f;
	if (saturate(lightProjectTexCoords.x) == lightProjectTexCoords.x &&
		saturate(lightProjectTexCoords.y) == lightProjectTexCoords.y) {
		float shadowMapDepth = gShadowMap.Sample(gSampleShadowMap, lightProjectTexCoords).r;

		float lightDepthValue = pIn.lightViewProjPosition.z / pIn.lightViewProjPosition.w;
		lightDepthValue -= bias;

		// Are we obscured by the shadow map.
		if (shadowMapDepth < lightDepthValue) {
			bCalculateLighting = false;
		}
	}*/

	if (bCalculateLighting) {
		float3 finalNormal = normalize(pIn.normal);
		if (gLight.type == 0) {
			lightColour = ParallelLight(finalNormal, material, gLight, toEye);
		}
		else if (gLight.type == 1) {
			lightColour = PointLight(pIn.position.xyz, finalNormal, material, gLight, toEye);
		}
		else {
			lightColour = SpotLight(pIn.position.xyz, finalNormal, material, gLight, toEye);
		}
	}

	lightColour += diffuse.xyz * gAmbientLight.xyz;

	return float4(lightColour, 1.0f);
}
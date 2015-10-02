
#include "LightingHelper.hlsli"

cbuffer constantBuffer {
	Light gLight;
	float3 gEyePosition;
	float4 gAmbientLight;
	float gFogStart;
	float gFogRange;
	float4 gFogColour;
};

static const float gDepthColourStart = 0.0f;
static const float gDepthColourFull = 0.56f;
static const float gDepthAlphaStart = 0.0f;
static const float gDepthAlphaFull = 0.7f;
static const float gDepthFoamStart = 0.42f;
static const float gDepthFoamFull = 0.413f;
static const float4 gDepthMidColour = float4(0.2666f, 0.835f, 0.745f, 1.0f);
static const float4 gDepthFullColour = float4(0.11764f, 0.32549f, 0.52157f, 1.0f);

Texture2D gFoamMap;
Texture2D gSpecularMap;
Texture2D gNormalMap;
Texture2D gHeightMap;
Texture2D gRefraction;
Texture2D gReflection;
Texture2D gShadowMap;
//Texture2DMS<float4, 8> gRefraction;
//Texture2DMS<float4, 8> gReflection;
SamplerState gSampleFoamMap;
SamplerState gSampleSpecularMap;
SamplerState gSampleNormalMap;
SamplerState gSampleHeightMap;
SamplerState gSampleRefraction;
SamplerState gSampleReflection;
SamplerState gSampleShadowMap;

struct PS_INPUT {
	float4 position : SV_POSITION;
	float4 positionScreen : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 waveCoord0 : TEXCOORD1;
	float2 waveCoord1 : TEXCOORD2;
	float2 waveCoord2 : TEXCOORD3;
	float2 waveCoord3 : TEXCOORD4;
	float2 heightMapCoord : TEXCOORD5;
	float4 reflectionPosition : TEXCOORD6;
	float3 positionWorld : TEXCOORD7;
	float4 lightViewPosition : TEXCOORD8;
};

float3 GetBumpedNormal(float3 vertNormal, float3 normalMapSample, float3 tangent)
{
	// Build orthogonal basis.
	float3 N = vertNormal;
	float3 T = normalize(tangent - dot(tangent, vertNormal) * vertNormal);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	return mul(normalMapSample, TBN);
}

float Fresnel(float nDotL, float fresnelBias, float fresnelPow)
{
	float facing = (1.0f - nDotL);
	return max(fresnelBias + (1.0 - fresnelBias) * pow(facing, fresnelPow), 0.0);
}

float4 main(PS_INPUT pIn) : SV_TARGET
{
	pIn.normal = normalize(pIn.normal);

	float3 toEye = gEyePosition - pIn.positionWorld.xyz;
	float distanceToEye = length(toEye);
	toEye /= distanceToEye;

	float4 specular = gSpecularMap.Sample(gSampleSpecularMap, pIn.waveCoord0.xy);

	// Normal mapping.
	float3 normalMapSample0 = gNormalMap.Sample(gSampleNormalMap, pIn.waveCoord0.xy).xyz;
	float3 normalMapSample1 = gNormalMap.Sample(gSampleNormalMap, pIn.waveCoord1).xyz;
	float3 normalMapSample2 = gNormalMap.Sample(gSampleNormalMap, pIn.waveCoord2).xyz;
	float3 normalMapSample3 = gNormalMap.Sample(gSampleNormalMap, pIn.waveCoord3).xyz;

	// Average the normals from the maps.
	float3 combinedNormalMapSample = normalize(2.0f * (normalMapSample0 + normalMapSample1 + normalMapSample2 + normalMapSample3) - 4.0f);
	float3 finalNormal = GetBumpedNormal(pIn.normal, combinedNormalMapSample, pIn.tangent);

	// Apply normal scale for reflection and refraction.
	float3 reflectionNormal = combinedNormalMapSample * float3(0.01f, 0.01f, 1.0f);
	float3 refractionNormal = combinedNormalMapSample * float3(0.03f, 0.03f, 1.0f);

	float2 refractionCoords;
	refractionCoords.x = pIn.positionScreen.x / pIn.positionScreen.w / 2.0f + 0.5f;
	refractionCoords.y = -pIn.positionScreen.y / pIn.positionScreen.w / 2.0f + 0.5f;

	// Get reflection colour.
	float2 reflectionCoords;
	reflectionCoords.x = pIn.reflectionPosition.x / pIn.reflectionPosition.w / 2.0f + 0.5f;
	reflectionCoords.y = -pIn.reflectionPosition.y / pIn.reflectionPosition.w / 2.0f + 0.5f;

	float4 reflection = gReflection.Sample(gSampleReflection, reflectionCoords.xy + reflectionNormal.xy);
	
	// Compute fresnel term.
	float nDotL = max(dot(toEye.yyy, combinedNormalMapSample), 0.0f);
	float fresnel = Fresnel(nDotL, 0.2f, 5.0f);

	reflection = fresnel * reflection;

	// Calculate refraction.
	float4 refraction = gRefraction.Sample(gSampleRefraction, refractionCoords + refractionNormal.xy);

	// Blend in colours based on the depth of the water.
	float depth = 1.0f - gHeightMap.Sample(gSampleHeightMap, pIn.heightMapCoord.xy).r;
	float depthColourBlend = 0.0f;
	if (depth >= gDepthColourStart) {
		depthColourBlend = saturate((depth - gDepthColourStart) / (gDepthColourFull - gDepthColourStart));
	}

	float depthAlphaBlend = 0.0f;
	if (depth >= gDepthAlphaStart) {
		depthAlphaBlend = saturate((depth - gDepthAlphaStart) / (gDepthAlphaFull - gDepthAlphaStart));
	}

	float4 waterColour = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f), gDepthMidColour, saturate(depthColourBlend / 0.5f));
	waterColour = lerp(waterColour, gDepthFullColour, saturate((depthColourBlend - 0.5f) / 0.5f));

	// Blend in foam.
	if (depth < gDepthFoamStart) {
		float4 foam = gFoamMap.Sample(gSampleFoamMap, pIn.waveCoord0.xy);
		float foamBlend = saturate((depth - gDepthFoamStart) / (gDepthFoamFull - gDepthFoamStart));
		waterColour = lerp(waterColour, float4(1.0f, 1.0f, 1.0f, 1.0f), foamBlend * foam.r);
	}

	waterColour = lerp(refraction, waterColour, depthAlphaBlend);
	
	// Add reflection and refraction to get the final colour.
	float4 finalColour = reflection + waterColour;

	bool bCalculateLighting = true;

	// Shadow mapping.
	/*
	// Calculate shadow mapping coords.
	float2 lightProjectTexCoords;
	lightProjectTexCoords.x = pIn.lightViewPosition.x / pIn.lightViewPosition.w / 2.0f + 0.5f;
	lightProjectTexCoords.y = -pIn.lightViewPosition.y / pIn.lightViewPosition.w / 2.0f + 0.5f;
	if (saturate(lightProjectTexCoords.x) == lightProjectTexCoords.x &&
		saturate(lightProjectTexCoords.y) == lightProjectTexCoords.y) {
		float shadowMapDepth = gShadowMap.Sample(gSampleShadowMap, lightProjectTexCoords).r;

		float lightDepthValue = pIn.lightViewPosition.z / pIn.lightViewPosition.w;
		lightDepthValue -= bias;

		// Are we obscured by the shadow map.
		if (shadowMapDepth < lightDepthValue) {
			bCalculateLighting = false;
		}
	}*/

	float3 lightColour = float3(0.0f, 0.0f, 0.0f);
	
	if (bCalculateLighting) {
		Material material = { finalColour, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 133.0f };

		if (gLight.type == 0) {
			lightColour = ParallelLight(finalNormal, material, gLight, toEye);
		}
		else if (gLight.type == 1) {
			lightColour = PointLight(pIn.positionWorld.xyz, finalNormal, material, gLight, toEye);
		}
		else {
			lightColour = SpotLight(pIn.positionWorld.xyz, finalNormal, material, gLight, toEye);
		}
	}

	// Ambient Lighting.
	lightColour += finalColour.xyz * gAmbientLight.xyz;

	// Fog
	float fogLerp = saturate((distanceToEye - gFogStart) / gFogRange);
	lightColour = lerp(lightColour, gFogColour.xyz, fogLerp);

	return float4(lightColour, 1.0f);
}
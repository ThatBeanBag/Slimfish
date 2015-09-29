
#include "LightingHelper.hlsli"

cbuffer constantBuffer {
	Light gLight;
	float3 gEyePosition;
	float4 gAmbientLight;
	float gFogStart;
	float gFogRange;
	float4 gFogColour;
};

Texture2D gDiffuseMap;
Texture2D gSpecularMap;
Texture2D gNormalMap;
Texture2D gRefraction;
Texture2D gReflection;
//Texture2DMS<float4, 8> gRefraction;
//Texture2DMS<float4, 8> gReflection;
SamplerState gSampleDiffuseMap;
SamplerState gSampleSpecularMap;
SamplerState gSampleNormalMap;
SamplerState gSampleRefraction;
SamplerState gSampleReflection;

struct PS_INPUT {
	float4 position : SV_POSITION;
	float4 positionScreen : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 waveCoord0 : TEXCOORD1;
	float2 waveCoord1 : TEXCOORD2;
	float2 waveCoord2 : TEXCOORD3;
	float2 waveCoord3 : TEXCOORD4;
	float4 reflectionPosition : TEXCOORD5;
	float3 positionWorld : TEXCOORD6;
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

	float4 diffuse = gDiffuseMap.Sample(gSampleDiffuseMap, pIn.waveCoord0.xy);
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

	//float3 reflectMapDimensions;
	//gReflection.GetDimensions(reflectMapDimensions.x, reflectMapDimensions.y, reflectMapDimensions.z);
	//float4 reflection = gReflection.Load(pIn.position.xy + reflectionNormal.xy * reflectMapDimensions.xy, reflectMapDimensions.z);
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
	float facing = (1.0f - nDotL);
	float fresnel = Fresnel(nDotL, 0.2f, 5.0f);

	reflection = fresnel * reflection;

	// Calculate refraction.

	float4 refractionA = gRefraction.Sample(gSampleRefraction, refractionCoords + refractionNormal.xy);
	float4 refractionB = gRefraction.Sample(gSampleRefraction, refractionCoords);
	/*float3 refracMapDimensions;
	gRefraction.GetDimensions(refracMapDimensions.x, refracMapDimensions.y, refracMapDimensions.z);
	float4 refractionA = gRefraction.Load((pIn.position.xy + refractionNormal.xy * refracMapDimensions.xy), refracMapDimensions.z);
	float4 refractionB = gRefraction.Load((pIn.position.xy), refracMapDimensions.z);*/

	//float4 refraction = lerp(refractionA, refractionB, refractionA.w);

	float4 waterColour = diffuse * float4(0.5f, 0.5f, 0.5f, 0.5f);

	float fDistScale = saturate(pIn.position.w * 0.001f);
	float4 waterDeepColour = lerp(refractionA, waterColour, fDistScale);
	waterColour = lerp(waterColour, waterDeepColour, nDotL);
	
	//float4 combinedColour = lerp(reflection, refractionA, nDotL);
	float4 combinedColour = reflection + waterColour;

	// Add refraction and reflection to get our final colour.
	float4 finalColour = lerp(combinedColour, diffuse, 0.2f);
	finalColour = combinedColour;

	// Lighting.
	//SurfaceInfo v = { pIn.positionWorld.xyz, finalNormal, finalColour, specular };
	Material material = { finalColour, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 133.0f };

	float3 lightColour;
	if (gLight.type == 0) {
		lightColour = ParallelLight(finalNormal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.positionWorld.xyz, finalNormal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.positionWorld.xyz, finalNormal, material, gLight, toEye);
	}

	lightColour += finalColour.xyz * gAmbientLight.xyz;

	// Fog
	float fogLerp = saturate((distanceToEye - gFogStart) / gFogRange);
	lightColour = lerp(lightColour, gFogColour.xyz, fogLerp);

	return float4(lightColour, diffuse.a);
}
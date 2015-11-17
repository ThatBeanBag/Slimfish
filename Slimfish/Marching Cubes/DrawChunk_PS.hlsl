#include "LightingHelper.hlsli"

cbuffer CBLighting {
	Light gLight;
	float3 gAmbientLight;
	float3 gEyePosition;
	float gFogStart;
	float gFogRange;
	float4 gFogColour;
};

struct PSInput {
	float4 hPosition : SV_POSITION;
	float4 wPosition : TEXCOORD;
	float3 wNormal : NORMAL;
};

Texture2D gStandardTexture;
Texture2D gTransitionTexture;
Texture2D gCliffTexture;

Texture2D gStandardNormalMap;
Texture2D gTransitionNormalMap;
Texture2D gCliffNormalMap;

SamplerState gSampleDiffuse;
SamplerState gSampleNoise;

static const float gCliffStart = 0.5f;
static const float gCliffFull = 0.7f;

static const float gTransitionStart = 0.2f;
static const float gTransitionFull = 0.5f;

float GetLerpAmount(float start, float full, float value)
{
	return saturate((value - start) / (full - start));
}

float4 SampleTriPlanar(Texture2D tex, float3 position, float3 blendVal)
{
	float4 texX = tex.Sample(gSampleDiffuse, position.yz * 0.1f);
	float4 texY = tex.Sample(gSampleDiffuse, position.xz * 0.1f);
	float4 texZ = tex.Sample(gSampleDiffuse, position.xy * 0.1f);
	// Blend the planar projections.
	return texX * blendVal.x + texY * blendVal.y + texZ * blendVal.z;
}

float3 GetBumpedNormal(float3 vertNormal, float3 normalMapSample, float3 tangent)
{
	// Uncompress the normal map sample from [0,1] to [-1,1]
	float3 uncompressedNormalMapSample = 2.0f * normalMapSample - 1.0f;

	// Build orthogonal basis.
	float3 N = vertNormal;
	float3 T = normalize(tangent - dot(tangent, vertNormal) * vertNormal);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	return mul(uncompressedNormalMapSample, TBN);
}

float4 main(PSInput pIn) : SV_TARGET
{
	float3 toEye = gEyePosition - pIn.wPosition.xyz;
	float distanceToEye = length(toEye);
	toEye /= distanceToEye;

	float4 specular = float4(0.1f, 0.1f, 0.1f, 1.0f);
	float3 lightColour = float3(0.0f, 0.0f, 0.0f);

	float3 finalNormal = normalize(pIn.wNormal);

	float3 triPlanarBlendVal = abs(finalNormal);
	float b = (triPlanarBlendVal.x + triPlanarBlendVal.y + triPlanarBlendVal.z);
	triPlanarBlendVal /= b;

	float4 diffuse = SampleTriPlanar(gStandardTexture, pIn.wPosition.xyz, triPlanarBlendVal);
	float3 normalSample = SampleTriPlanar(gStandardNormalMap, pIn.wPosition.xyz, triPlanarBlendVal).xyz;

	// Blend in the cliff and transition textures.
	//float incline = abs(finalNormal.x);
	float incline = 1.0f - max(dot(pIn.wNormal, float3(0.0f, 1.0f, 0.0f)), 0.0f);
	if (incline > gTransitionStart) {
		float transitionBlend = GetLerpAmount(gTransitionStart, gTransitionFull, incline);
		float4 transitionDiffuse = SampleTriPlanar(gTransitionTexture, pIn.wPosition.xyz, triPlanarBlendVal);
		float3 transitionNormalSample = SampleTriPlanar(gTransitionNormalMap, pIn.wPosition.xyz, triPlanarBlendVal).xyz;

		diffuse = lerp(diffuse, transitionDiffuse, transitionBlend);
		normalSample = lerp(normalSample, transitionNormalSample, transitionBlend);

		if (incline > gCliffStart) {
			float cliffBlend = GetLerpAmount(gCliffStart, gCliffFull, incline);
			float4 cliffDiffuse = SampleTriPlanar(gCliffTexture, pIn.wPosition.xyz, triPlanarBlendVal);
				float3 cliffNormalSample = SampleTriPlanar(gCliffNormalMap, pIn.wPosition.xyz, triPlanarBlendVal).xyz;

			specular = lerp(specular, float4(0.7f, 0.7f, 0.7f, 1.0f), cliffBlend);
			diffuse = lerp(diffuse, cliffDiffuse, cliffBlend);
			normalSample = lerp(normalSample, cliffNormalSample, cliffBlend);
		}
	}

	//diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3 uncompressedNormalSample = 2.0f * normalSample - 1.0f;
	//finalNormal = normalize(normalSample + uncompressedNormalSample);
	finalNormal = GetBumpedNormal(finalNormal, normalSample, float3(0, 1, 0));

	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 40.0f };

	if (gLight.type == 0) {
		lightColour = ParallelLight(finalNormal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}

	// Add ambient light and occlude it.
	float ambo = pIn.wPosition.w;
	ambo = saturate(ambo + 0.2f); // Boost the ambient occlusion a little so that we don't get any dark blacks.
	lightColour += diffuse.xyz * gAmbientLight.xyz * ambo;

	float fogLerp = saturate((distanceToEye - gFogStart) / gFogRange);
	lightColour = lerp(lightColour, gFogColour, fogLerp);

	return float4(lightColour, 1.0f);
}
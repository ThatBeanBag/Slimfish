#include "LightingHelper.hlsli"

cbuffer CBLighting {
	Light gLight;
	float3 gAmbientLight;
	float3 gEyePosition;
	float gFogStart;
	float gFogRange;
	float4 gFogColour;
	float gAmbientOcclusionInfluence;
};

struct PSInput {
	float4 hPosition : SV_POSITION;
	float3 wNormal : NORMAL;
	float4 wPosition : TEXCOORD0;
	float4 lPosition : TEXCOORD1;
};

Texture2D gShadowMapTexture;
Texture2D gStandardTexture;
Texture2D gTransitionTexture;
Texture2D gCliffTexture;

Texture2D gStandardNormalMap;
Texture2D gTransitionNormalMap;
Texture2D gCliffNormalMap;

SamplerState gSampleShadowMap;
SamplerState gSampleDiffuse;
SamplerState gSampleNoise;

static const float gCliffStart = 0.5f;
static const float gCliffFull = 0.7f;

static const float gTransitionStart = 0.2f;
static const float gTransitionFull = 0.5f;

static const float gShadowMapBias = 0.008f; //0.008f;
static const float gShadowMapSize = 4096.0f;
static const float gInvShadowMapSize = 1.0f / gShadowMapSize;

static const float2 gShadowMapOffsets[9] = {
	float2(-1.0f, -1.0f),
	float2(-1.0f, 0.0f),
	float2(-1.0f, 1.0f),
	float2(0.0f, -1.0f),
	float2(0.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, -1.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f),
};

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

float CalculateShadowFactor(float4 lightPosition)
{
	// Transform the position to NDC space.
	lightPosition.xyz /= lightPosition.w;

	// Project the coordinates into texture space.
	lightPosition.x = 0.5 * lightPosition.x + 0.5f;
	lightPosition.y = -0.5 * lightPosition.y + 0.5f;

	float depth = lightPosition.z / lightPosition.w;

	float lightAmount = 0.0f;
	[unroll]
	for (int i = 0; i < 9; ++i) {
		float2 uv = lightPosition.xy + gShadowMapOffsets[i] * gInvShadowMapSize;
		float shadowMapDepth = gShadowMapTexture.Sample(gSampleShadowMap, uv).r;
		lightAmount += depth <= (shadowMapDepth + gShadowMapBias);
	}

	return lightAmount / 9.0f;
}

float4 main(PSInput pIn) : SV_TARGET
{
	float3 toEye = gEyePosition - pIn.wPosition.xyz;
	float distanceToEye = length(toEye);
	toEye /= distanceToEye;

	float4 specular = float4(0.1f, 0.1f, 0.1f, 1.0f);

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

			specular = lerp(specular, float4(0.3f, 0.3f, 0.3f, 1.0f), cliffBlend);
			diffuse = lerp(diffuse, cliffDiffuse, cliffBlend);
			normalSample = lerp(normalSample, cliffNormalSample, cliffBlend);
		}
	}

	float3 uncompressedNormalSample = 2.0f * normalSample - 1.0f;
	finalNormal = GetBumpedNormal(finalNormal, normalSample, float3(0, 1, 0));

	// Boost the ambient occlusion a little so that we don't get any dark blacks.
	float ambientOcclusion = saturate(pIn.wPosition.w + 0.2f);
	ambientOcclusion = lerp(1.0f, ambientOcclusion, gAmbientOcclusionInfluence);

	// Add ambient light and occlude it.
	float3 finalColour = diffuse.xyz * gAmbientLight.xyz * ambientOcclusion;

	// Calculate shadows.
	float shadow = CalculateShadowFactor(pIn.lPosition);

	// Calculate lighting.
	float3 lightColour = 0.0f;
	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 30.0f };

	if (gLight.type == 0) {
		lightColour = ParallelLight(finalNormal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}

	finalColour += lightColour * shadow;

	// Calculate fog.
	float fogLerp = saturate((distanceToEye - gFogStart) / gFogRange);
	finalColour = lerp(finalColour, gFogColour.rgb, fogLerp);

	return float4(finalColour, 1.0f);
}
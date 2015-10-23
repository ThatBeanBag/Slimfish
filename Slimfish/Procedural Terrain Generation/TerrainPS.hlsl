
#include "LightingHelper.hlsli"

cbuffer constantBuffer {
	Light gLight;
	float3 gEyePosition;
	float4 gAmbientLight;
	float gFogStart;
	float gFogRange;
	float4 gFogColour;
};

static const float gCliffStart = 0.2f;
static const float gCliffFull = 0.35f;

static const float gSandStart = 4.0f;
static const float gSandFull = 2.0f;

static const float gDetailStart = 0.998f;
static const float gDetailFull = 0.996f;

Texture2D gGrassTexture;
Texture2D gCliffTexture;
Texture2D gSandTexture;
Texture2D gGrassNormalMap;
Texture2D gCliffNormalMap;
Texture2D gSandNormalMap;
SamplerState gSample0;
SamplerState gSample1;
SamplerState gSample2;
SamplerState gSample3;
SamplerState gSample4;
SamplerState gSample5;

struct PS_INPUT {
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float4 positionDepth : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD1;
};

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

float4 main(PS_INPUT pIn) : SV_TARGET
{
	float3 toEye = gEyePosition - pIn.positionWorld.xyz;
	float distanceToEye = length(toEye);
	toEye /= distanceToEye;

	pIn.normal = normalize(pIn.normal);
	pIn.tangent = normalize(pIn.tangent);

	float4 grass = gGrassTexture.Sample(gSample0, pIn.texCoord);
	float4 sand = gSandTexture.Sample(gSample1, pIn.texCoord);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float3 normalSample = gGrassNormalMap.Sample(gSample3, pIn.texCoord).xyz;

	// Blend in the sand.
	float sandBlend = 0.0f;
	if (pIn.positionWorld.y < gSandStart) {
		sandBlend = saturate((pIn.positionWorld.y - gSandStart) / (gSandFull - gSandStart));

		// Calculate normal for sand.
		float3 sandNormal = gSandTexture.Sample(gSample5, pIn.texCoord).xyz;
		normalSample = lerp(normalSample, sandNormal, sandBlend);
		//normalSample += sandNormal * sandBlend;
	}

	float4 diffuse = lerp(grass, sand, sandBlend);

	// Blend in the cliff.
	float cliffBlend = 0.0f;
	float incline = 1.0f - max(dot(pIn.normal, float3(0.0f, 1.0f, 0.0f)), 0.0f);
	if (incline >= gCliffStart) {
		cliffBlend = saturate((incline - gCliffStart) / (gCliffFull - gCliffStart));

		// Calculate tri-planar blending.
		float3 triPlanarBlending = abs(pIn.normal);
		float b = (triPlanarBlending.x + triPlanarBlending.y + triPlanarBlending.z);
		triPlanarBlending /= b;

		// Get the planar projections.
		float4 cliffX = gCliffTexture.Sample(gSample1, pIn.positionWorld.yz * 0.05f);
		float4 cliffY = gCliffTexture.Sample(gSample1, pIn.positionWorld.xz * 0.05f);
		float4 cliffZ = gCliffTexture.Sample(gSample1, pIn.positionWorld.xy * 0.05f);
		// Blend the planar projections.
		float4 cliff = cliffX * triPlanarBlending.x + cliffY * triPlanarBlending.y + cliffZ * triPlanarBlending.z;

		diffuse = lerp(diffuse, cliff, cliffBlend);

		// Calculate normal mapping for cliff.
		// Get the planar projections.
		float3 cliffNormalX = gCliffNormalMap.Sample(gSample4, pIn.positionWorld.yz * 0.05f).xyz;
		float3 cliffNormalY = gCliffNormalMap.Sample(gSample4, pIn.positionWorld.xz * 0.05f).xyz;
		float3 cliffNormalZ = gCliffNormalMap.Sample(gSample4, pIn.positionWorld.xy * 0.05f).xyz;
		// Blend the planar projections.
		float3 cliffNormal = cliffNormalX * triPlanarBlending.x + cliffNormalY * triPlanarBlending.y + cliffNormalZ * triPlanarBlending.z;

		normalSample = lerp(normalSample, cliffNormal, cliffBlend);
		//normalSample += cliffNormal * cliffBlend;
	}

	// Normal mapping.
	float depthValue = pIn.positionDepth.z / pIn.positionDepth.w;

	float3 finalNormal = pIn.normal;
	//if (depthValue < gDetailStart) {
	//}
	//float normalBlend = saturate((depthValue - gDetailStart) / (gDetailFull - gDetailStart));
	//float3 bumpedNormal = GetBumpedNormal(pIn.normal, normalSample, pIn.tangent);
	//finalNormal = lerp(pIn.normal, bumpedNormal, normalBlend);
	//finalNormal = bumpedNormal;

	// Lighting.
	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f };

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

	lightColour += diffuse.xyz * gAmbientLight.xyz;

	// Fog
	float fogLerp = saturate((distanceToEye - gFogStart) / gFogRange);
	lightColour = lerp(lightColour.xyz, gFogColour.xyz, fogLerp);

	return float4(lightColour, diffuse.a);
}
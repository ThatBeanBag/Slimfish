
#include "LightingHelper.hlsli"

cbuffer constantBuffer {
	Light gLight;
	float3 gEyePosition;
	float4 gAmbientLight;
	float gFogStart;
	float gFogRange;
	float4 gFogColour;
};

Texture2D gGrassTexture;
Texture2D gCliffTexture;
Texture2D gSpecularMap;
SamplerState gSample0;
SamplerState gSample1;
SamplerState gSample2;

struct PS_INPUT {
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	float clip : SV_ClipDistance0;
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
	float4 specular = gSpecularMap.Sample(gSample2, pIn.texCoord);

	float3 triPlanarBlending = abs(pIn.normal);
	float b = (triPlanarBlending.x + triPlanarBlending.y + triPlanarBlending.z);
	triPlanarBlending /= b; 

	// Get the planar projections.
	float4 cliffX = gCliffTexture.Sample(gSample1, pIn.positionWorld.yz * 0.1f);
	float4 cliffY = gCliffTexture.Sample(gSample1, pIn.positionWorld.xz * 0.1f);
	float4 cliffZ = gCliffTexture.Sample(gSample1, pIn.positionWorld.xy * 0.1f);
	// Blend the planar projections.
	float4 cliff = cliffX * triPlanarBlending.x + cliffY * triPlanarBlending.y + cliffZ * triPlanarBlending.z;


	// Normal mapping.
	/*float3 normalMapSample = gNormalMap.Sample(gSample2, pIn.texCoord);
	float3 normalMapSample2 = gNormalMap.Sample(gSample2, pIn.texCoord2);

	//float3 combinedNormalMapSample = normalize(normalMapSample + normalMapSample2);
	float3 combinedNormalMapSample = normalMapSample;

	float3 finalNormal = GetBumpedNormal(pIn.normal, combinedNormalMapSample, pIn.tangent);
	float3 finalNormal = normalMapSample + normalize((2.0f * normalMapSample - 1.0f) + pIn.normal);*/

	// Blend in the sand.
	float4 sand = float4(0.85f, 0.73f, 0.65f, 1.0f);
	const float sandStart = 3.0f;
	const float sandFull =	1.0f;
	float sandBlend = 0.0f;
	if (pIn.positionWorld.y < sandStart) {
		sandBlend = saturate((pIn.positionWorld.y - sandStart) / (sandFull - sandStart));
	}

	float4	diffuse = lerp(grass, sand, sandBlend);

	// Blend in the cliff.
	const float cliffStart = 0.15f;
	const float cliffFull = 0.3f;

	float blend = 0.0f;

	float incline = 1.0f - max(dot(pIn.normal, float3(0.0f, 1.0f, 0.0f)), 0.0f);
	if (incline >= cliffStart) {
		blend = saturate((incline - cliffStart) / (cliffFull - cliffStart));
	}
	
	diffuse = lerp(diffuse, cliff, blend);

	// Lighting.
	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f };

	float3 lightColour;
	if (gLight.type == 0) {
		lightColour = ParallelLight(pIn.normal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.positionWorld.xyz, pIn.normal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.positionWorld.xyz, pIn.normal, material, gLight, toEye);
	}

	lightColour += diffuse.xyz * gAmbientLight.xyz;

	// Fog
	float fogLerp = saturate((distanceToEye - gFogStart) / gFogRange);
	lightColour = lerp(lightColour, gFogColour, fogLerp);

	return float4(lightColour, diffuse.a);
}
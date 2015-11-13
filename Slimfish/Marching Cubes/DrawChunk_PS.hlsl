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

Texture2D gTexture;
SamplerState gSample0;

float4 main(PSInput pIn) : SV_TARGET
{
	float3 toEye = gEyePosition - pIn.wPosition.xyz;
	float distanceToEye = length(toEye);
	toEye /= distanceToEye;

	//float4 diffuse = float4(0.5f, 0.5f, 0.0f, 1.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float3 lightColour = float3(0.0f, 0.0f, 0.0f);

	float3 finalNormal = normalize(pIn.wNormal);

	float3 triPlanarBlending = abs(finalNormal);
	float b = (triPlanarBlending.x + triPlanarBlending.y + triPlanarBlending.z);
	triPlanarBlending /= b;

	// Get the planar projections.
	float4 texX = gTexture.Sample(gSample0, pIn.wPosition.yz * 0.5f);
	float4 texY = gTexture.Sample(gSample0, pIn.wPosition.xz * 0.5f);
	float4 texZ = gTexture.Sample(gSample0, pIn.wPosition.xy * 0.5f);
		// Blend the planar projections.
	float4 diffuse = texX * triPlanarBlending.x + texY * triPlanarBlending.y + texZ * triPlanarBlending.z;

	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 10.0f };

	if (gLight.type == 0) {
		lightColour = ParallelLight(finalNormal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}

	float ambo = pIn.wPosition.w;
	lightColour += diffuse.xyz * gAmbientLight.xyz * ambo;

	float fogLerp = saturate((distanceToEye - gFogStart) / gFogRange);
	lightColour = lerp(lightColour, gFogColour, fogLerp);

	return float4(lightColour, 1.0f);
}
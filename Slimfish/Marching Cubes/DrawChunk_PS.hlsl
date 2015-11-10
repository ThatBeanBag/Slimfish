#include "LightingHelper.hlsli"

cbuffer CBLighting {
	Light gLight;
	float3 gAmbientLight;
	float3 gEyePosition;
};

struct PSInput {
	float4 hPosition : SV_POSITION;
	float4 wPosition : TEXCOORD;
	float3 wNormal : NORMAL;
};

float4 main(PSInput pIn) : SV_TARGET
{
	float3 toEye = gEyePosition - pIn.wPosition.xyz;
	float distanceToEye = length(toEye);
	toEye /= distanceToEye;

	float4 diffuse = float4(0.5f, 0.5f, 0.0f, 1.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 10.0f };

	float3 lightColour = float3(0.0f, 0.0f, 0.0f);

	float3 finalNormal = normalize(pIn.wNormal);
	if (gLight.type == 0) {
		lightColour = ParallelLight(finalNormal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.wPosition.xyz, finalNormal, material, gLight, toEye);
	}

	lightColour += diffuse.xyz * gAmbientLight.xyz;



	return float4(lightColour, 1.0f);
}
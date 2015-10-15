
#include "LightingHelper.hlsli"

cbuffer constantBuffer {
	Light gLight;
	float3 gAmbientLight;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

float4 main(VSOutput pIn) : SV_TARGET
{
	float3 finalNormal = normalize(pIn.normal);

	float4 diffuse = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float3 toEye = float3(0.0f, 1.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	Material material = { diffuse, specular, float4(0.0f, 0.0f, 0.0f, 0.0f), 133.0f };

	float3 lightColour = float3(0.0f, 0.0f, 0.0f);

	if (gLight.type == 0) {
		lightColour = ParallelLight(finalNormal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.position.xyz, finalNormal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.position.xyz, finalNormal, material, gLight, toEye);
	}

	lightColour += diffuse.xyz * gAmbientLight.xyz;

	return float4(lightColour, 1.0f);
}
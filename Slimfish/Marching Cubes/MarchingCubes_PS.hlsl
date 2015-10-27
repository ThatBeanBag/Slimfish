
#include "LightingHelper.hlsli"

struct PSInput {
	float4 position : SV_POSITION;
	float4 wPosition : TEXCOORD;
	float4 colour : COLOR;
};

cbuffer CBLighting {
	Light gLight;
	float3 gEyePosition = float3(0.0f, 0.0f, 0.0f);
	float3 gAmbientLight = float3(0.1f, 0.1f, 0.1f);
};

cbuffer CBLod {
	float gVoxelDim = 65;
	float gVoxelDimMinusOne = 64;
	float gWVoxelSize = 1.0f / 64.0f;
	float gWChunkSize = 4.0f;
	float gInvVoxelDim = 1.0f / 65.0f;
	float gInvVoxelDimMinusOne = 1.0f / 64.0f;
	float gMargin = 4.0f;
	float gVoxelDimPlusMargins = 73.0f;
	float gVoxelDimPlusMarginsMinusOne = 72.0f;
	float gInvVoxelDimPlusMargins = 1.0f / 73.0f;
	float gInvVoxelDimPlusMarginsMinusOne = 1.0f / 72.0f;
};

cbuffer CBChunk {
	float3 gWChunkPosition = float3(0, 0, 0);
};

Texture3D gTexture3DDensity;
SamplerState gSamplerPoint;

float4 main(PSInput pIn) : SV_TARGET
{
	float3 uvw = pIn.position.xyz - gWChunkPosition;
	float2 invVoxelDimPlusMargins = float2(gInvVoxelDimPlusMargins, 0.0f);

	// Calculate normal.
	float3 gradient;
	gradient.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMargins.xyy, 0) -
				 gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimPlusMargins.xyy, 0);
	gradient.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMargins.yxy, 0) -
				 gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimPlusMargins.yxy, 0);
	gradient.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMargins.yyx, 0) -
				 gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimPlusMargins.yyx, 0);
	float3 normal = -normalize(gradient);

	// Calculate lighting.
	Material material = { pIn.colour, float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f };

	float3 lightColour = float3(0.0f, 0.0f, 0.0f);

	float3 toEye = gEyePosition - pIn.wPosition.xyz;
	float distanceToEye = length(toEye);
	toEye /= distanceToEye;

	if (gLight.type == 0) {
		lightColour = ParallelLight(normal, material, gLight, toEye);
	}
	else if (gLight.type == 1) {
		lightColour = PointLight(pIn.position.xyz, normal, material, gLight, toEye);
	}
	else {
		lightColour = SpotLight(pIn.position.xyz, normal, material, gLight, toEye);
	}

	lightColour += pIn.colour.xyz * gAmbientLight.xyz;

	return float4(lightColour, 1.0f);
}
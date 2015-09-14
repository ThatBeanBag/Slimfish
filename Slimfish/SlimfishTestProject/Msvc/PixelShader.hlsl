
#include "LightingHelper.hlsli"

cbuffer cbPerFrame {
	Light gLight;
	float3 gEyePosition;
	float4 gAmbientLight;
};

Texture2D gDiffuseTexture;
Texture2D gSpecularMap;
Texture2D gNormalMap;
SamplerState gSample0;
SamplerState gSample1;
SamplerState gSample2;

struct PS_INPUT {
	float4 positionScreen : SV_POSITION;
	float3 positionWorld : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 tangent : TANGENT;
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
	pIn.normal = normalize(pIn.normal);

	float4 diffuse = gDiffuseTexture.Sample(gSample0, pIn.texCoord);
	float4 specular = gSpecularMap.Sample(gSample1, pIn.texCoord);

	// Normal mapping.
	float3 normalMapSample = gNormalMap.Sample(gSample2, pIn.texCoord);
	float3 finalNormal = GetBumpedNormal(pIn.normal, normalMapSample, pIn.tangent);

	// Lighting.
	SurfaceInfo v = { pIn.positionScreen.xyz, finalNormal, diffuse, specular };

	float3 lightColour;
	if (gLight.m_Type == 0) {
		lightColour = ParallelLight(v, gLight, gEyePosition);
	}
	else if (gLight.m_Type == 1) {
		lightColour = PointLight(v, gLight, gEyePosition);
	}
	else {
		lightColour = SpotLight(v, gLight, gEyePosition);
	}

	lightColour += diffuse * gAmbientLight;

	return float4(lightColour, diffuse.a);
}
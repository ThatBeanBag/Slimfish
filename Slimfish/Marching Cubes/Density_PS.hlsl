cbuffer lodCostantBuffer {
	float gVoxelDim = 65;
	float gVoxelDimMinusOne = 64;
	float2 gWVoxelSize = float2(1.0f / 64.0f, 0.0f);
	float gWChunkSize = 4.0f;
	float2 gInvVoxelDim = float2(1.0f / 65.0f, 0.0f);
	float2 gInvVoxelDimMinusOne = float2(1.0f / 64.0f, 0.0f);
	float gMargin = 4.0f;
	float gVoxelDimPlusMargins = 73.0f;
	float gVoxelDimPlusMarginsMinusOne = 72.0f;
	float2 gInvVoxelDimPlusMargins = float2(1.0f / 73.0f, 0.0f);
	float2 gInvVoxelDimPlusMarginsMinusOne = float2(1.0f / 72.0f, 0.0f);
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 wPosition : TEXCOORD0;
	float3 chunkPosition : TEXCOORD1;
};

Texture3D gTexture3DNoise0;
Texture3D gTexture3DNoise1;
Texture3D gTexture3DNoise2;
Texture3D gTexture3DNoise3;
SamplerState gSamplerTrilinearWrap;

#include "Density.hlsli"

float4 main( PSInput pIn ) : SV_TARGET
{
	return float4(Density(pIn.wPosition.xyz), 0.0f, 0.0f, 1.0f);
}
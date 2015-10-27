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

float main( PSInput pIn ) : SV_TARGET0
{
	float density = Density(pIn.wPosition.xyz);
	return density;
}
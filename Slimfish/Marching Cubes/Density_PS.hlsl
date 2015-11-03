
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

#include "Chunk.hlsli"
#include "Density.hlsli"

float main( PSInput pIn ) : SV_TARGET0
{
	float density = Density(pIn.wPosition.xyz);
	return density;
}
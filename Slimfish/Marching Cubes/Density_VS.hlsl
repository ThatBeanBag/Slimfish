
#include "Chunk.hlsli"

struct VSInput {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	uint nInstanceID : SV_InstanceID;
};

struct VSOutput {
	float4 position : POSITION;
	float4 wPosition : TEXCOORD0;
	float3 chunkPosition : TEXCOORD1;
	uint nInstanceID : BLAH;
};

VSOutput main( VSInput vIn ) 
{
	VSOutput vOut;

	float3 chunkPosition = float3(vIn.texCoord, float(vIn.nInstanceID) * gInvVoxelDimPlusMargins);

	chunkPosition.xyz *= gVoxelDim.x * gInvVoxelDimMinusOne;
	//chunkPosition.xy *= gVoxelDimPlusMargins * gInvVoxelDimPlusMarginsMinusOne;

	float3 extChunkPosition = (chunkPosition * gVoxelDimPlusMargins - gMargin) * gInvVoxelDim;

	vOut.wPosition = float4(gWChunkPosition + extChunkPosition * gWChunkSize, 1.0f);
	vOut.position = float4(vIn.position.xy, 0.5f, 1.0f);
	vOut.position.y = -vOut.position.y;	// Flip Y coords.
	vOut.chunkPosition = chunkPosition;
	vOut.nInstanceID = vIn.nInstanceID;

	return vOut;
}
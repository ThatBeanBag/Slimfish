
cbuffer CBChunk {
	float3 gWChunkPosition = float3(0.0f, 0.0f, 0.0f);
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

struct VSInput {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	uint nInstanceID : SV_InstanceID;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float4 wPosition : TEXCOORD0;
	float3 chunkPosition : TEXCOORD1;
	uint nInstanceID : BLAH;
};

VSOutput main( VSInput vIn ) 
{
	VSOutput vOut;

	float3 chunkPosition = float3(vIn.texCoord.x, vIn.texCoord.y, vIn.nInstanceID * gInvVoxelDimPlusMargins.x);
	chunkPosition *= gVoxelDim.x * gInvVoxelDimPlusMarginsMinusOne.x;

	float3 extChunkPosition = (chunkPosition * gVoxelDimPlusMargins.x - gMargin) * gInvVoxelDim.x;

	vOut.wPosition = float4(gWChunkPosition + extChunkPosition * gWChunkSize, 1.0f);
	vOut.position = float4(vIn.position.xy, 0.5f, 1.0f);
	vOut.position.y = -vOut.position.y;	// Flip Y coords.
	vOut.chunkPosition = chunkPosition;
	vOut.nInstanceID = vIn.nInstanceID;

	return vOut;
}
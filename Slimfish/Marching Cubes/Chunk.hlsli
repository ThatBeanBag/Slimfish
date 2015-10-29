
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

#include "Chunk.hlsli"

struct VSInput {
	uint z8_y8_x8_null4_edgeNum4 : TEXCOORD;
	uint nVertexID : SV_VertexID;
};

struct VSOutput {
	float4 projCoord : POSITION;
	uint nVertexID : TEXCOORD0;
	uint slice : TEXCOORD1;
};

VSOutput main(VSInput vIn) {
	VSOuput vOut;

	// Unpack the position and edge number.
	uint edgeNum = vIn.z8_y8_x8_null4_edgeNum4 & 0x0f;
	int3 position = (int3)((vIn.z8_y8_x8_null4_edgeNum4.xxx >> uint3(8, 16, 24)) & 0xff);

	// Vertices are on either edge 3, 0, or 8.
	position.x *= 3;
	if (edgeNum == 0) {
		position.x += 1;
	}
	if (edgeNum == 8) {
		position.x += 2;
	}

	float2 uv = (float2)position.xy;
	// Fix the alignment for point sampling.
	uv.x += position.x + 0.5f * gInvVoxelDim / 3.0f;
	uv.y += position.y + 0.5f * gInvVoxelDim;

	// Get the projected coordinates in the range [-1, 1] and flip the y component.
	vOut.projCoord.x = (uv.x * gInvVoxelDim / 3.0f) * 2 - 1;
	vOut.projCoord.y = -((uv.y * gInvVoxelDim) * 2 - 1);
	vOut.projCoord.z = 0;
	vOut.projCoord.w = 1;

	// Get the vertex ID and slice.
	vOut.nVertexID = vIn.nVertexID;
	vOut.slice = position.z;

	return vOut;
}

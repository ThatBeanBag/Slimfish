
#include "Chunk.hlsli"

struct VSInput {
	uint z8_y8_x8_null4_edgeNum4: TEX;
	uint nVertexID : SV_VertexID;
};

struct VSOutput {
	float4 projCoord : SV_POSITION;
	uint2 vertexID : VERTEXID;
	uint slice : SLICE;
};

VSOutput main(VSInput vIn) {
	VSOutput vOut;

	// Unpack the position and edge number.
	uint edgeNum = vIn.z8_y8_x8_null4_edgeNum4 & 0x0f;
	uint3 position;
	position.x = (vIn.z8_y8_x8_null4_edgeNum4 >> 8) & 0xff;
	position.y = (vIn.z8_y8_x8_null4_edgeNum4 >> 16) & 0xff;
	position.z = (vIn.z8_y8_x8_null4_edgeNum4 >> 24) & 0xff;

	// Vertices are on either edge 3, 0, or 8.
	position.x *= 3;
	if (edgeNum == 1) {
		position.x += 0;
	}
	if (edgeNum == 0) {
		position.x += 1;
	}
	if (edgeNum == 8) {
		position.x += 2;
	}

	float2 uv = (float2)position.xy;
	// Fix the alignment for point sampling.
	uv.x += 0.5f * gInvVoxelDim / 3.0f;
	uv.y += 0.5f * gInvVoxelDim / 1.0f;

	// Get the projected coordinates in the range [-1, 1] and flip the y component.
	vOut.projCoord = float4(
		(uv.x * gInvVoxelDim / 3.0f) * 2 - 1,
		-((uv.y * gInvVoxelDim) * 2 - 1),
		0.0f,
		1.0f);

	// Get the vertex ID and slice.
	vOut.vertexID = vIn.nVertexID;
	vOut.slice = position.z;

	return vOut;
}

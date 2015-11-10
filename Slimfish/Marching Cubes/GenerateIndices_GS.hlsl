#include "Chunk.hlsli"

struct GSInput {
	uint z8_y8_x8_case8 : TEX;
};

struct GSOutput {
	uint index : TEXCOORD;
};

cbuffer CBTables {
	float4 gEdgeStart[12];
	int4 gEdgeAxis[12];
	int4 gTriTable[4096 / 4]; // The triangle table for lookup when create polys.
};

Texture3D<uint> gTexture3DVertexID;

int GetTriTableValue(uint i, uint j)
{
	if (i >= 256 || j >= 16) {
		return -1;
	}

	uint index = (i * 16) + j;

	return gTriTable[index / 4][index % 4];
}


[maxvertexcount(15)]
void main(point GSInput gIn[1], inout TriangleStream< GSOutput > outputStream)
{
	uint cubeCase = gIn[0].z8_y8_x8_case8 & 0xff;
	int3 position = (int3)((gIn[0].z8_y8_x8_case8.xxx >> uint3(8, 16, 24)) & 0xff);

	// Don't generate polys in the final layer.
	if (max(max(position.x, position.y), position.z) < (int)gVoxelDimMinusOne) {
		// Generate indices for each triangle.
		for (uint i = 0; GetTriTableValue(cubeCase, i) != -1; i +=3) {
			// Get the edge numbers for the triangle.
			int3 edgeNumbers = float3(
				GetTriTableValue(cubeCase, i),
				GetTriTableValue(cubeCase, i + 1),
				GetTriTableValue(cubeCase, i + 2));

			int3 edgePosition;
			int3 indices;

			// Calculate the indices for this triangle.
			edgePosition = position + (int3)gEdgeStart[edgeNumbers.x];
			edgePosition.x = edgePosition.x * 3.0f + gEdgeAxis[edgeNumbers.x].x;
			indices.x = gTexture3DVertexID.Load(int4(edgePosition, 0)).x;

			edgePosition = position + (int3)gEdgeStart[edgeNumbers.y];
			edgePosition.x = edgePosition.x * 3.0f + gEdgeAxis[edgeNumbers.y].x;
			indices.y = gTexture3DVertexID.Load(int4(edgePosition, 0)).x;

			edgePosition = position + (int3)gEdgeStart[edgeNumbers.z];
			edgePosition.x = edgePosition.x * 3.0f + gEdgeAxis[edgeNumbers.z].x;
			indices.z = gTexture3DVertexID.Load(int4(edgePosition, 0)).x;

			// Add the indices of this triangle to the stream.
			if (indices.x * indices.y * indices.z != 0) {
				GSOutput gOut;
				gOut.index = indices.x;
				outputStream.Append(gOut);

				gOut.index = indices.y;
				outputStream.Append(gOut);

				gOut.index = indices.z;
				outputStream.Append(gOut);

				outputStream.RestartStrip();
			}
		}
	}
}

/*
#include "Chunk.hlsli"

struct GSInput {
	uint z8_y8_x8_case8 : POSITION;
};

struct GSOutput {
	uint index : TEXCOORD;
};

cbuffer CBTables {
	//	uint gCaseToNumPolys[256];
	float4 gEdgeStart[12];
	float4 gEdgeDirection[12];
	float4 gEdgeEnd[12];
	int4 gEdgeAxis[12];
	int4 gTriTable[4096 / 4 + !!(4096 % 4)]; // The triangle table for lookup when create polys.
};

Texture3D<uint> gTexture3DVertexID;

int GetTriTableValue(uint i, uint j)
{
	if (i >= 256 || j >= 16) {
		return -1;
	}

	uint index = (i * 16) + j;

	return gTriTable[index / 4][index % 4];
}


[maxvertexcount(15)]
void main(point GSInput gIn[1] : SV_POSITION, inout TriangleStream< GSOutput > outputStream)
{
	uint cubeCase = gIn[0].z8_y8_x8_case8 & 0xff;
	int3 position = (int3)((gIn[0].z8_y8_x8_case8.xxx >> uint3(8, 16, 24)) & 0xff);

		// Don't generate polys in the final layer.
		if (max(max(position.x, position.y), position.z) >= (int)gVoxelDimMinusOne) {
			return;
		}
		else {
			// Generate indices for each triangle.
			for (uint i = 0; GetTriTableValue(cubeCase, i) != -1; i += 3) {
				// Get the edge numbers for the triangle.
				int3 edgeNumbers = float3(
					GetTriTableValue(cubeCase, i),
					GetTriTableValue(cubeCase, i + 1),
					GetTriTableValue(cubeCase, i + 2));

				int3 edgePosition;
				int3 indices;

				// Calculate the indices for this triangle.
				edgePosition = position + (int3)gEdgeStart[edgeNumbers.x];
				edgePosition.x = edgePosition.x * 3.0f + gEdgeAxis[edgeNumbers.x].x;
				indices.x = gTexture3DVertexID.Load(int4(edgePosition, 0)).x;

				edgePosition = position + (int3)gEdgeStart[edgeNumbers.y];
				edgePosition.x = edgePosition.x * 3.0f + gEdgeAxis[edgeNumbers.y].x;
				indices.y = gTexture3DVertexID.Load(int4(edgePosition, 0)).x;

				edgePosition = position + (int3)gEdgeStart[edgeNumbers.z];
				edgePosition.x = edgePosition.x * 3.0f + gEdgeAxis[edgeNumbers.z].x;
				indices.z = gTexture3DVertexID.Load(int4(edgePosition, 0)).x;

				// Add the indices of this triangle to the stream.
				GSOutput gOut;
				gOut.index = indices.x;
				outputStream.Append(gOut);

				gOut.index = indices.y;
				outputStream.Append(gOut);

				gOut.index = indices.z;
				outputStream.Append(gOut);

				outputStream.RestartStrip();
			}
		}

}*/
struct GSInput {
	float4 position : SV_POSITION;
};

struct GSOutput {
	float4 position : SV_POSITION;
	float4 wPosition : TEXCOORD;
	float4 colour : COLOR;
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

cbuffer CBChunk {
	float3 gWChunkPosition = float3(0, 0, 0);
};

cbuffer CBPerFrame {
	float4x4 gWorldViewProjectionMatrix;
};

cbuffer CBTriTable {
	int gTriTable[4096]; // The triangle table for lookup when create polys.
};

Texture3D gTexture3DDensity;
SamplerState gSamplerPoint;

static const float3 gCorners[8] = {
	float3(0.0f, 0.0f, 0.0f),
	float3(1.0f, 0.0f, 0.0f),
	float3(1.0f, 1.0f, 0.0f),
	float3(0.0f, 1.0f, 0.0f),
	float3(0.0f, 0.0f, 1.0f),
	float3(1.0f, 0.0f, 1.0f),
	float3(1.0f, 1.0f, 1.0f),
	float3(0.0f, 1.0f, 1.0f)
};

/** Interpolate between the given points by a specific amount
	used to find the vertex position on the cube's edge.
*/
float3 VertexInterp(float isoLevel, float3 v0, float l0, float3 v1, float l1)
{
	float lerper = (isoLevel - l0) / (l1 - l0);
	return lerp(v0, v1, lerper);
}

int GetTriTableValue(int i, int j)
{
	if (i >= 256 || j >= 16) {
		return -1;
	}

	return gTriTable[(i * 16) + j];
}

[maxvertexcount(18)]
void main(point GSInput input[1], inout TriangleStream<GSOutput> triStream)
{
	float isoLevel = 0.5f;

	float3 cubePoses[8];
	float cubeVals[8];
	for (int i = 0; i < 8; ++i) {
		cubePoses[i] = input[0].position.xyz + (gCorners[i] /** gWVoxelSize*/);
		cubeVals[i] = gTexture3DDensity.SampleLevel(gSamplerPoint, (cubePoses[i] + 1.0f) / 2.0f, 0);
		cubePoses[i] += gWChunkPosition;
	}

	int cubeIndex = 0;
	cubeIndex = int(cubeVals[0] < isoLevel);
	cubeIndex += int(cubeVals[1] < isoLevel) * 2;
	cubeIndex += int(cubeVals[2] < isoLevel) * 4;
	cubeIndex += int(cubeVals[3] < isoLevel) * 8;
	cubeIndex += int(cubeVals[4] < isoLevel) * 16;
	cubeIndex += int(cubeVals[5] < isoLevel) * 32;
	cubeIndex += int(cubeVals[6] < isoLevel) * 64;
	cubeIndex += int(cubeVals[7] < isoLevel) * 128;

	GSOutput output;
	float4 colour = float4(0.0f, 0.0f, 1.0f, 1.0f);
	float3 relativePosition = (input[0].position.xyz + 1.0f) / 2.0f;
	float3 center = float3(0.5f, 0.5f, 0.5f);
	float distanceToCentre = length(center - relativePosition);
	float maxDistance = 1.65f;

	float relativeDistance = distanceToCentre / maxDistance;
	float colourg = relativeDistance;

	colour.g = colourg;
	colour.b -= colourg * 3.0f;
	output.colour = colour;

	if (cubeIndex != 0 && cubeIndex != 255) {
		float3 vertList[12];

		vertList[0] = VertexInterp(isoLevel, cubePoses[0], cubeVals[0], cubePoses[1], cubeVals[1]);
		vertList[1] = VertexInterp(isoLevel, cubePoses[1], cubeVals[1], cubePoses[2], cubeVals[2]);
		vertList[2] = VertexInterp(isoLevel, cubePoses[2], cubeVals[2], cubePoses[3], cubeVals[3]);
		vertList[3] = VertexInterp(isoLevel, cubePoses[3], cubeVals[3], cubePoses[0], cubeVals[0]);
		vertList[4] = VertexInterp(isoLevel, cubePoses[4], cubeVals[4], cubePoses[5], cubeVals[5]);
		vertList[5] = VertexInterp(isoLevel, cubePoses[5], cubeVals[5], cubePoses[6], cubeVals[6]);
		vertList[6] = VertexInterp(isoLevel, cubePoses[6], cubeVals[6], cubePoses[7], cubeVals[7]);
		vertList[7] = VertexInterp(isoLevel, cubePoses[7], cubeVals[7], cubePoses[4], cubeVals[4]);
		vertList[8] = VertexInterp(isoLevel, cubePoses[0], cubeVals[0], cubePoses[4], cubeVals[4]);
		vertList[9] = VertexInterp(isoLevel, cubePoses[1], cubeVals[1], cubePoses[5], cubeVals[5]);
		vertList[10] = VertexInterp(isoLevel, cubePoses[2], cubeVals[2], cubePoses[6], cubeVals[6]);
		vertList[11] = VertexInterp(isoLevel, cubePoses[3], cubeVals[3], cubePoses[7], cubeVals[7]);

		GSOutput point1;
		GSOutput point2;
		GSOutput point3;
		point1.colour = colour;
		point2.colour = colour;
		point3.colour = colour;

		for (int i = 0; GetTriTableValue(cubeIndex, i) != -1; i += 3)
		{
			//Add vertices to the output stream
			point1.wPosition = float4(vertList[GetTriTableValue(cubeIndex, i)], 1.0f);
			point1.position = mul(point1.wPosition, gWorldViewProjectionMatrix);

			point2.wPosition = float4(vertList[GetTriTableValue(cubeIndex, i + 1)], 1.0f);
			point2.position = mul(point2.wPosition, gWorldViewProjectionMatrix);

			point3.wPosition = float4(vertList[GetTriTableValue(cubeIndex, i + 2)], 1.0f);
			point3.position = mul(point3.wPosition, gWorldViewProjectionMatrix);

			triStream.Append(point1);
			triStream.Append(point2);
			triStream.Append(point3);
			triStream.RestartStrip();
		}
	}
}
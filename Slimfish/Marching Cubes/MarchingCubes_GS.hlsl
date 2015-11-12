#include "Chunk.hlsli"

struct GSInput {
	float4 position : SV_POSITION;
};

struct GSOutput {
	float4 position : POSITION;
	float3 normal : NORMAL;
};

cbuffer CBTriTable {
	int4 gTriTable[4096 / 4 + !!(4096%4)]; // The triangle table for lookup when create polys.
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

	int index = (i * 16) + j;

	return gTriTable[index/4][index%4];
}

float3 GetNormal(float3 uvw)
{
	float2 invVoxelDimAndZero = float2(gInvVoxelDim, 0.0f);

	float3 gradient;
	gradient.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimAndZero.xyy, 0) -
				 gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimAndZero.xyy, 0);
	gradient.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimAndZero.yxy, 0) -
				 gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimAndZero.yxy, 0);
	gradient.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimAndZero.yyx, 0) -
				 gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimAndZero.yyx, 0);
	float3 normal = -normalize(gradient);
	return normal;
}

[maxvertexcount(18)]
void main(point GSInput input[1], inout TriangleStream<GSOutput> triStream)
{
	float isoLevel = 0.5f;

	float3 normal = GetNormal(input[0].position.xyz);
	float3 normals[8];
	float3 cubePoses[8];
	float cubeVals[8];
	for (int i = 0; i < 8; ++i) {
		float3 uvw = (input[0].position.xyz * gVoxelDimMinusOne * gInvVoxelDim)  + (gCorners[i] * gInvVoxelDimMinusOne);
		normals[i] = GetNormal(uvw);
		cubeVals[i] = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw, 0);

		// Scale by the size of the chunk and add the chunk position.
		cubePoses[i] = uvw * gWChunkSize + gWChunkPosition;
	}

	// Get the cube case.
	int cubeCase = 0;
	cubeCase = int(cubeVals[0] < isoLevel);
	cubeCase += int(cubeVals[1] < isoLevel) * 2;
	cubeCase += int(cubeVals[2] < isoLevel) * 4;
	cubeCase += int(cubeVals[3] < isoLevel) * 8;
	cubeCase += int(cubeVals[4] < isoLevel) * 16;
	cubeCase += int(cubeVals[5] < isoLevel) * 32;
	cubeCase += int(cubeVals[6] < isoLevel) * 64;
	cubeCase += int(cubeVals[7] < isoLevel) * 128;

	/*GSOutput output;
	float4 colour = float4(0.0f, 0.0f, 1.0f, 1.0f);
	float3 relativePosition = (input[0].position.xyz + 1.0f) / 2.0f;
	float3 center = float3(0.5f, 0.5f, 0.5f);
	float distanceToCentre = length(center - relativePosition);
	float maxDistance = 1.65f;

	float relativeDistance = distanceToCentre / maxDistance;
	float colourg = relativeDistance;

	colour.g = colourg;
	colour.b -= colourg * 3.0f;
	output.colour = colour;*/

	if (cubeCase != 0 && cubeCase != 255) {
		float3 vertList[12];
		float3 normalList[12];

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

		normalList[0] = VertexInterp(isoLevel, normals[0], cubeVals[0], normals[1], cubeVals[1]);
		normalList[1] = VertexInterp(isoLevel, normals[1], cubeVals[1], normals[2], cubeVals[2]);
		normalList[2] = VertexInterp(isoLevel, normals[2], cubeVals[2], normals[3], cubeVals[3]);
		normalList[3] = VertexInterp(isoLevel, normals[3], cubeVals[3], normals[0], cubeVals[0]);
		normalList[4] = VertexInterp(isoLevel, normals[4], cubeVals[4], normals[5], cubeVals[5]);
		normalList[5] = VertexInterp(isoLevel, normals[5], cubeVals[5], normals[6], cubeVals[6]);
		normalList[6] = VertexInterp(isoLevel, normals[6], cubeVals[6], normals[7], cubeVals[7]);
		normalList[7] = VertexInterp(isoLevel, normals[7], cubeVals[7], normals[4], cubeVals[4]);
		normalList[8] = VertexInterp(isoLevel, normals[0], cubeVals[0], normals[4], cubeVals[4]);
		normalList[9] = VertexInterp(isoLevel, normals[1], cubeVals[1], normals[5], cubeVals[5]);
		normalList[10] = VertexInterp(isoLevel, normals[2], cubeVals[2], normals[6], cubeVals[6]);
		normalList[11] = VertexInterp(isoLevel, normals[3], cubeVals[3], normals[7], cubeVals[7]);

		GSOutput point1;
		GSOutput point2;
		GSOutput point3;
		point1.normal = normal;
		point2.normal = normal;
		point3.normal = normal;

		for (int i = 0; GetTriTableValue(cubeCase, i) != -1; i += 3)
		{
			//Add vertices to the output stream
			point1.position = float4(vertList[GetTriTableValue(cubeCase, i)], 1.0f);
			point1.normal = normalList[GetTriTableValue(cubeCase, i)];
			point2.position = float4(vertList[GetTriTableValue(cubeCase, i + 1)], 1.0f);
			point2.normal = normalList[GetTriTableValue(cubeCase, i + 1)];
			point3.position = float4(vertList[GetTriTableValue(cubeCase, i + 2)], 1.0f);
			point3.normal = normalList[GetTriTableValue(cubeCase, i + 2)];

			triStream.Append(point1);
			triStream.Append(point2);
			triStream.Append(point3);
			triStream.RestartStrip();
		}
	}
}
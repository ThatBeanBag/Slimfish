#include "Chunk.hlsli"

struct VSInput {
	uint z8_y8_x8_null4_edgeNum4: TEXCOORD;
};

struct VSOutput {
	float4 wPositionAmbOcc : TEXCOORD;
	float3 wNormal : NORMAL;
};

Texture3D gTexture3DDensity;
SamplerState gSamplerPoint;

cbuffer gTriTable {
	uint gCaseToNumPolys[256];
	float3 gEdgeStart[12];
	float3 gEdgeDirection[12];
	float3 gEdgeEnd[12];
	uint gEdgeAxis[12];
};

VSOutput PlaceVertexOnEdge(float3 wPosition, float3 uvw, int edgeNum)
{
	VSOuput vOut;
	
	// Get the density values at the ends of the edge.
	float density0 = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMarginsMinusOne * gEdgeStart[edgeNum], 0);
	float density1 = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMarginsMinusOne * gEdgeEnd[edgeNum], 0);
	// interpolate between the the densities to find the point at which the density is 0.
	float t = saturate(density0 / (density0 - density1));

	float3 position = gEdgeStart[edgeNum] + t * gEdgeDirection[edgeNum];
	vOut.wPositionAmbOcc = wPosition + position * gWVoxelSize;
	uvw = uvw + position * gInvVoxelDimPlusMarginsMinusOne;

	// TODO: Generate ambient occlusion.

	float3 gradient;
	grad.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMargins.xyy, 0)
		   - gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - gInvVoxelDimPlusMargins.xyy, 0);
	grad.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMargins.yxy, 0)
		   - gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - gInvVoxelDimPlusMargins.yxy, 0);
	grad.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMargins.yyx, 0)
		   - gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - gInvVoxelDimPlusMargins.yyx, 0);
	vOut.wNormal = -normalize(gradient);

	return vOut;
}

VSOuput main(VSInput vIn) 
{
	uint3 upackedPosition;
	unpackedPosition.x = (vIn.z8_y8_x8_null4_edgeNum4 >> 8) & 0xff;
	unpackedPosition.y = (vIn.z8_y8_x8_null4_edgeNum4 >> 16) & 0xff;
	unpackedPosition.z = (vIn.z8_y8_x8_null4_edgeNum4 >> 24) & 0xff;

	float3 chunkWritePosition = (float3)unpackedPosition * gInvVoxelDimMinusOne;
	float3 chunkReadPosition = (gMargin + gVoxelDimMinusOne * chunkWritePosition) * gInvVoxelDimPlusMarginsMinusOne;

	float3 wPosition = gWChunkPosition + chunkWritePosition * gWChunkSize;

	float3 uvw = chunkReadPosition + gInvVoxelDimPlusMarginsMinusOne;
	uvw *= (gVoxelDimPlusMargins - 1) * gInvVoxelDimPlusMargins;

	// Unpack the edge number.
	int edgeNum = vIn.z8_y8_x8_null4_edgeNum4 & 0x0f;
	return PlaceVertexOnEdge(wPosition, uvw, edgeNum);
}
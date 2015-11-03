#include "Chunk.hlsli"

struct VSInput {
	uint z8_y8_x8_null4_edgeNum4: POSITION;
};

struct VSOutput {
	float4 wPositionAmbOcc : POSITION;
	float3 wNormal : NORMAL;
};

Texture3D gTexture3DDensity;
SamplerState gSamplerPoint;

cbuffer CBTables {
	//uint gCaseToNumPolys[256];
	float4 gEdgeStart[12];
	float4 gEdgeDirection[12];
	float4 gEdgeEnd[12];
	int4 gEdgeAxis[12];
};

VSOutput PlaceVertexOnEdge(float3 wPosition, float3 uvw, int edgeNum)
{
	VSOutput vOut;
	
	// Get the density values at the ends of the edge.
	float density0 = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMarginsMinusOne * gEdgeStart[edgeNum].xyz, 0).x;
	float density1 = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMarginsMinusOne * gEdgeEnd[edgeNum].xyz, 0).x;
	// interpolate between the the densities to find the point at which the density is 0.
	float t = saturate(density0 / (density0 - density1));

	float3 position = gEdgeStart[edgeNum].xyz + t * gEdgeDirection[edgeNum].xyz;
	vOut.wPositionAmbOcc = float4(wPosition + position * gWVoxelSize, 1.0f);
	uvw = uvw + position * gInvVoxelDimPlusMarginsMinusOne;

	// TODO: Generate ambient occlusion.

	float2 invVoxelDimPlusMarginsAndZero = float2(gInvVoxelDimPlusMargins, 0.0f);

	float3 gradient;
	gradient.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsAndZero.xyy, 0).x
			   - gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimPlusMarginsAndZero.xyy, 0).x;
	gradient.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsAndZero.yxy, 0).x
			   - gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimPlusMarginsAndZero.yxy, 0).x;
	gradient.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsAndZero.yyx, 0).x
			   - gTexture3DDensity.SampleLevel(gSamplerPoint, uvw - invVoxelDimPlusMarginsAndZero.yyx, 0).x;
	vOut.wNormal = -normalize(gradient);

	return vOut;
}

VSOutput main(VSInput vIn) 
{
	uint3 unpackedPosition;
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
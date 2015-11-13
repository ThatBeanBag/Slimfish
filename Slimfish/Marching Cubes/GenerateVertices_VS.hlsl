#include "Chunk.hlsli"

struct VSInput {
	uint z8_y8_x8_null4_edgeNum4: TEX;
};

struct VSOutput {
	float4 wPositionAmbOcc : POSITION;
	float3 wNormal : NORMAL;
};

Texture3D<float> gTexture3DDensity;
SamplerState gSamplerPoint;

cbuffer CBTables {
	float4 gEdgeStart[12];
	float4 gEdgeEnd[12];
};

cbuffer CBAmbientOcclusion {
	float4 gAmboDistance[16];
	float4 gOcclusion[16];
	float4 gRayDirections[256];
};

Texture3D gTexture3DNoise0;
Texture3D gTexture3DNoise1;
Texture3D gTexture3DNoise2;
Texture3D gTexture3DNoise3;
SamplerState gSamplerTrilinearWrap;

#include "Density.hlsli"

#define AMBO_RAYS 64
#define AMBO_STEPS 16

float GetAmbientOcclusion(float3 wPosition, float3 uvw)
{
	float cellsToSkipAtRayStart = 1.25f;
	float amboRayDistanceCells = gVoxelDimPlusMargins * 0.25f;
	float3 invVoxelDimTweaked = gInvVoxelDimPlusMargins * gVoxelDimPlusMargins / 160.0f;

	float ambo = 0.0f;

	for (int i = 0; i < AMBO_RAYS; ++i) {
		float3 rayDirection = gRayDirections[i].xyz;
		float3 ray = uvw + rayDirection * gInvVoxelDimPlusMargins * cellsToSkipAtRayStart;
		float3 rayDelta = rayDirection * invVoxelDimTweaked * amboRayDistanceCells;

		float amboCurrent = 1.0f;

		// Short range ambient rays.
		rayDelta *= (1.0f / AMBO_STEPS);
		for (int j = 0; j < AMBO_STEPS; ++j) {
			ray += rayDelta;
			float t = gTexture3DDensity.SampleLevel(gSamplerPoint, ray, 0);
			amboCurrent = lerp(amboCurrent, 0, saturate(t * 6.0f) * gOcclusion[j].z);
		}

		// Long range ambient rays.
		for (int j = 0; j < 5; ++j) {
			float distance = (j + 2) / 5.0f;
			distance = pow(distance, 1.8f);
			distance *= 40.0f;
			float t = Density(wPosition + rayDirection * distance);
			const float shadow_hardness = 0.5f;
			amboCurrent *= 0.1f + 0.9f * saturate(-t * shadow_hardness + 0.3f);
		}

		amboCurrent *= 1.4f;
		ambo += amboCurrent;

		/*for (int j = 1; j <= AMBO_STEPS; ++j) {
			float t = gTexture3DDensity.SampleLevel(gSamplerPoint, ray * i, 0);
			amboCurrent *= lerp(amboCurrent, 0, saturate(t * 6) * gOcclusion[j].z);
		}*/
	}

	ambo *= (1.0f / AMBO_RAYS);
	ambo *= 5.0f;

	return ambo;
}

VSOutput PlaceVertexOnEdge(float3 wPosition, float3 uvw, int edgeNum)
{
	VSOutput vOut;
	
	// Get the density values at the ends of the edge.
	float density0 = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMarginsMinusOne * gEdgeStart[edgeNum].xyz, 0);
	float density1 = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + gInvVoxelDimPlusMarginsMinusOne * gEdgeEnd[edgeNum].xyz, 0);
	// Interpolate between the the densities to find the point at which the density is 0 on the edge.
	float t = saturate(density0 / (density0 - density1));

	//float3 position = gEdgeStart[edgeNum].xyz + t * gEdgeDirection[edgeNum].xyz;
	float3 position = lerp(gEdgeStart[edgeNum].xyz, gEdgeEnd[edgeNum], t);
	vOut.wPositionAmbOcc = float4(wPosition + position * gWVoxelSize, 1.0f);
	uvw += position * gInvVoxelDimPlusMarginsMinusOne;

	// TODO: Generate ambient occlusion.
	vOut.wPositionAmbOcc.w = GetAmbientOcclusion(vOut.wPositionAmbOcc.xyz, uvw);

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

	float3 uvw = chunkReadPosition + gInvVoxelDimPlusMarginsMinusOne * 0.125f;
	uvw *= (gVoxelDimPlusMargins - 1) * gInvVoxelDimPlusMargins;

	// Unpack the edge number.
	int edgeNum = vIn.z8_y8_x8_null4_edgeNum4 & 0x0f;
	return PlaceVertexOnEdge(wPosition, uvw, edgeNum);
}
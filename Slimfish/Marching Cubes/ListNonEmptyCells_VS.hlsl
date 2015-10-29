
#include "Chunk.hlsli"

struct VSInput {
	float2 writeUV : POSITION0;
	float2 readUV : POSITION1;
	uint nInstanceID : SV_InstanceID;
};

struct VSOuput {
	uint z8_y8_x8_case8 : TEXCOORD;
};

Texture3D gTexture3DDensity;
SamplerState gSamplerPoint;

VSOuput main( VSInput vIn )
{
	VSOuput vOut;

	float3 chunkReadPosition = float3(vIn.readUV, (vIn.nInstanceID + gMargin) * gInvVoxelDimPlusMargins);
	float3 chunkWritePosition = float3(vIn.writeUV, (vIn.nInstanceID * gInvVoxelDim);

	float3 wPosition = gWChunkPosition + chunkWritePosition * gWChunkSize;
	float3 uvw = chunkReadPosition + gInvVoxelDimPlusMarginsMinusOne.xxx * 0.125f;
	uvw.xy *= gVoxelDimPlusMarginsMinusOne * gInvVoxelDimPlusMargins;

	float4 field0123;
	float4 field4567;

	field0123.x = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.yyy, 0).x;
	field0123.y = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.yxy, 0).x;
	field0123.z = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.xxy, 0).x;
	field0123.w = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.xyy, 0).x;

	field4567.x = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.yyx, 0).x;
	field4567.y = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.yxx, 0).x;
	field4567.z = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.xxx, 0).x;
	field4567.w = gTexture3DDensity.SampleLevel(NearestClamp, uvw + gInvVoxelDimPlusMarginsMinusOne.xyx, 0).x;

	uint4 i0123 = (uint4)saturate(field0123 * 99999);
	uint4 i4567 = (uint4)saturate(field4567 * 99999);

	int cubeCase = (i0123.x) | (i0123.y << 1) | (i0123.z << 2) | (i0123.w << 3) |
				   (i4567.x << 4) | (i4567.y << 5) | (i4567.z << 6) | (i4567.w << 7);

	uint3 uint3Position = uint3(vIn.writeUV * gVoxelDimMinusOne, vIn.nInstanceID);

	vOut.z8_y8_x8_case8 = (uint3coord.z << 24) |
						  (uint3coord.y << 16) |
						  (uint3coord.x << 8) |
						  (cubeCase);

	return vOut;
}
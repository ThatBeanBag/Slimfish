
#include "Chunk.hlsli"

struct VSInput {
	float2 writeUV : POSITION0;
	float2 readUV : POSITION1;
	uint nInstanceID : SV_InstanceID;
};

struct VSOuput {
	uint z8_y8_x8_case8 : POSITION;
};

Texture3D gTexture3DDensity;
SamplerState gSamplerPoint;

VSOuput main( VSInput vIn )
{
	VSOuput vOut;

	float3 chunkReadPosition = float3(vIn.readUV, (vIn.nInstanceID + gMargin) * gInvVoxelDimPlusMargins);
	float3 chunkWritePosition = float3(vIn.writeUV, (vIn.nInstanceID * gInvVoxelDim));

	float3 wPosition = gWChunkPosition + chunkWritePosition * gWChunkSize;
	float3 uvw = chunkReadPosition + gInvVoxelDimPlusMarginsMinusOne.xxx * 0.125f;
	uvw.xy *= gVoxelDimPlusMarginsMinusOne * gInvVoxelDimPlusMargins;

	float4 field0123;
	float4 field4567;
	
	float2 invVoxelDimPlusMarginsMinusOneAndZero = float2(gInvVoxelDimPlusMarginsMinusOne, 0.0f);
	
	// Get the eight corners of the cell.
	field0123.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yyy, 0).x;
	field0123.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yxy, 0).x;
	field0123.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xxy, 0).x;
	field0123.w = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xyy, 0).x;

	field4567.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yyx, 0).x;
	field4567.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yxx, 0).x;
	field4567.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xxx, 0).x;
	field4567.w = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xyx, 0).x;

	uint4 i0123 = (uint4)saturate(field0123 * 99999);
	uint4 i4567 = (uint4)saturate(field4567 * 99999);

	/*int cubeCase = int(field0123.x < isoLevel);
	cubeCase += int(field0123.y < isoLevel) * 2;
	cubeCase += int(field0123.z < isoLevel) * 4;
	cubeCase += int(field0123.w < isoLevel) * 8;
	cubeCase += int(field4567.x < isoLevel) * 16;
	cubeCase += int(field4567.y < isoLevel) * 32;
	cubeCase += int(field4567.z < isoLevel) * 64;
	cubeCase += int(field4567.w < isoLevel) * 128;*/

	int cubeCase = (i0123.x)	  | (i0123.y << 1) | (i0123.z << 2) | (i0123.w << 3) |
				   (i4567.x << 4) | (i4567.y << 5) | (i4567.z << 6) | (i4567.w << 7);

	uint3 uint3Position = uint3(vIn.writeUV * gVoxelDimMinusOne, vIn.nInstanceID);

	vOut.z8_y8_x8_case8 = (uint3Position.z << 24) |
						  (uint3Position.y << 16) |
						  (uint3Position.x << 8)  |
						  (cubeCase);
	//vOut.z8_y8_x8_case8 = cubeCase;
	return vOut;
}
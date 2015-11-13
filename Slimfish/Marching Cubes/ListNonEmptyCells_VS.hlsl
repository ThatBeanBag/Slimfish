
#include "Chunk.hlsli"

struct VSInput {
	float2 writeUV : POSITION0;
	float2 readUV : POSITION1;
	uint nInstanceID : SV_InstanceID;
};

struct VSOuput {
	uint z8_y8_x8_case8 : TEX;
};

Texture3D gTexture3DDensity;
SamplerState gSamplerPoint;

VSOuput main( VSInput vIn )
{
	VSOuput vOut;

	float3 chunkReadPosition = float3(vIn.readUV, (float(vIn.nInstanceID) + gMargin) * gInvVoxelDimPlusMargins);
	float3 chunkWritePosition = float3(vIn.writeUV, (float(vIn.nInstanceID) * gInvVoxelDim));

	float3 wPosition = gWChunkPosition + chunkWritePosition * gWChunkSize;
	float3 uvw = chunkReadPosition + gInvVoxelDimPlusMarginsMinusOne * 0.125f;
	uvw.xy *= gVoxelDimPlusMarginsMinusOne * gInvVoxelDimPlusMargins;
	
	float2 invVoxelDimPlusMarginsMinusOneAndZero = float2(gInvVoxelDimPlusMarginsMinusOne, 0.0f);
	float4 field0123;
	float4 field4567;
	
	// Get the eight corners of the cell.
	field0123.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yyy, 0).x;
	field0123.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yxy, 0).x;
	field0123.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xxy, 0).x;
	field0123.w = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xyy, 0).x;

	field4567.x = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yyx, 0).x;
	field4567.y = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.yxx, 0).x;
	field4567.z = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xxx, 0).x;
	field4567.w = gTexture3DDensity.SampleLevel(gSamplerPoint, uvw + invVoxelDimPlusMarginsMinusOneAndZero.xyx, 0).x;

	// Set the bits of the 8 corners. 
	// If the density is negative, set the bit to 0. 
	// If the density if positive, set it to 1.
	uint4 i0123 = (uint4)saturate(ceil(field0123));
	uint4 i4567 = (uint4)saturate(ceil(field4567));
	//uint4 i0123 = (uint4)saturate(field0123 * 99999);
	//uint4 i4567 = (uint4)saturate(field4567 * 99999);
	int cubeCase = (i0123.x)	  | (i0123.y << 1) | (i0123.z << 2) | (i0123.w << 3) |
				   (i4567.x << 4) | (i4567.y << 5) | (i4567.z << 6) | (i4567.w << 7);

	/*int cubeCase = 0;
	cubeCase += int(field0123.x < 0.5f) * 1;
	cubeCase += int(field0123.y < 0.5f) * 2;
	cubeCase += int(field0123.z < 0.5f) * 4;
	cubeCase += int(field0123.w < 0.5f) * 8;
	cubeCase += int(field4567.x < 0.5f) * 16;
	cubeCase += int(field4567.y < 0.5f) * 32;
	cubeCase += int(field4567.z < 0.5f) * 64;
	cubeCase += int(field4567.w < 0.5f) * 128;*/

	uint3 uint3Position = uint3(vIn.writeUV * gVoxelDimMinusOne, vIn.nInstanceID);

	vOut.z8_y8_x8_case8 = (uint3Position.z << 24) |
						  (uint3Position.y << 16) |
						  (uint3Position.x << 8)  |
						  (cubeCase);
	return vOut;
}
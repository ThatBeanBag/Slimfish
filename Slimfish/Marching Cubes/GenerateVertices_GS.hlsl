// This geometry shader is only for streaming out, it's effectively a pass through.

struct GSInput {
	float4 wPositionAmbOcc : POSITION;
	float3 wNormal : NORMAL;
};

struct GSOutput
{
	float4 wPositionAmbOcc : POSITION;
	float3 wNormal : NORMAL;
};

[maxvertexcount(1)]
void main(point GSInput input[1], inout PointStream< GSOutput > outputStream)
{
	// Just pass through.
	GSOutput gOut;
	gOut.wPositionAmbOcc = input[0].wPositionAmbOcc;
	gOut.wNormal = input[0].wNormal;
	outputStream.Append(gOut);
}
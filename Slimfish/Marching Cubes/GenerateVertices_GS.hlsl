// This geometry shader is only for streaming out, it's effectively a pass through.

struct GSInput {
	float4 wPositionAmbOcc : TEXCOORD;
	float3 wNormal : NORMAL;
};

struct GSOutput
{
	float4 wPositionAmbOcc : POSITION;
	float4 wNormal : NORMAL;
};

[maxvertexcount(1)]
void main(point GSInput input[1], inout PointStream< GSOutput > outputStream)
{
	// Just pass through.
	GSOutput gOut;
	gOut.wPositionAmbOcc = input[0].wPositionAmbOcc;
	gOut.wNormal = float4(input[0].wNormal, 0.0f);
	outputStream.Append(gOut);
}
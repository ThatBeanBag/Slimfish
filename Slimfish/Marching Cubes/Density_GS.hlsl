
struct GSInput {
	float4 position : POSITION;
	float4 wPosition : TEXCOORD0;
	float3 chunkPosition : TEXCOORD1;
	uint nInstanceID : BLAH;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float3 wPosition : TEXCOORD0;
	float3 chunkPosition : TEXCOORD1;
	uint renderTargetIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(3)]
void main( triangle GSInput gIn[3], inout TriangleStream< GSOutput > gOut)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.position = gIn[i].position;
		element.wPosition = gIn[i].wPosition;
		element.chunkPosition = gIn[i].chunkPosition;
		element.renderTargetIndex = gIn[i].nInstanceID;
		gOut.Append(element);
	}

	gOut.RestartStrip();
}
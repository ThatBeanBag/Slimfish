// The only reason for this geometry is to specify the SV_RenderTargetArrayIndex because
// the vertex shader cannot do this. We need this render slices of a 3D texture.

struct GSInput {
	float4 projCoord : POSITION;
	uint nVertexID : TEXCOORD0;
	uint slice : TEXCOORD1;
};

struct GSOutput {
	float4 projCoord : POSITION;
	uint nVertexID : TEXCOORD;
	uint renderTargetIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(1)]
void main( point GSInput gIn[1], inout PointStream< GSOutput > outputStream)
{
	// Just pass through.
	GSOutput gOut;
	gOut.projCoord = gIn[0].projCoord;
	gOut.nVertexID = gIn[0].nVertexID;
	gOut.renderTargetIndex = gIn[0].slice;

	outputStream.Append(gOut);
}
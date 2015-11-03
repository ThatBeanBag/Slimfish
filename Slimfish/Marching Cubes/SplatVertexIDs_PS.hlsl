
struct PSInput {
	float4 projCoord : SV_POSITION;
	uint nVertexID : VERTEXID;
	uint renderTargetIndex : SV_RenderTargetArrayIndex;
};

uint main(PSInput pIn) : SV_Target
{
	return pIn.nVertexID;
}

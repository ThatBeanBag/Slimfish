
struct PSInput {
	float4 projCoord : POSITION;
	uint nVertexID : TEXCOORD;
	uint renderTargetIndex : SV_RenderTargetArrayIndex;
};

uint main(PSInput pIn) : SV_Target
{
	return pIn.nVertexID;
}

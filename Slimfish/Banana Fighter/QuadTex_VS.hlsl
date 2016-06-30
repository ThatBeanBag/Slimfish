
struct VS_IN {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float4 colour : COLOUR;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float4 colour : COLOUR;
};

VS_OUTPUT main(VS_IN vIn)
{
	VS_OUTPUT vOut;

	vOut.position = float4(vIn.position, 1.0f);
	vOut.texCoord = vIn.texCoord;
	vOut.colour = vIn.colour;

	return vOut;
}
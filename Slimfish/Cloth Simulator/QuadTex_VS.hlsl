
struct VS_IN {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_IN vIn)
{
	VS_OUTPUT vOut;

	vOut.position = float4(vIn.position, 1.0f);
	vOut.texCoord = vIn.texCoord;

	return vOut;
}
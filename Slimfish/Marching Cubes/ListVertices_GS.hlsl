
struct GSInput {
	uint z8_y8_x8_null5_edgeFlags3 : TEXCOORD;
};

struct GSOutput
{
	uint z8_y8_x8_null4_edgeNum4 : TEXCOORD;
};


[maxvertexcount(3)]
void main(point GSInput input[1], inout PointStream< GSOutput > output)
{
	GSOutput gOut;

	uint z8_y8_x8_null8 = input[0].z8_y8_x8_null5_edgeFlags3 & 0xffffff00;

	// Stream out a marker point for each edge 3, 0, 8 that needs a vertex.
	if (input[0].z8_y8_x8_null5_edgeFlags3 & 1) {
		gOut.z8_y8_x8_null4_edgeNum4 = z8_y8_x8_null8 | 3;
		Stream.Append(gOut);
	}

	if (input[0].z8_y8_x8_null5_edgeFlags3 & 2) {
		gOut.z8_y8_x8_null4_edgeNum4 = z8_y8_x8_null8 | 0;
		Stream.Append(gOut);
	}
	
	if (input[0].z8_y8_x8_null5_edgeFlags3 & 4) {
		gOut.z8_y8_x8_null4_edgeNum4 = z8_y8_x8_null8 | 8;
		Stream.Append(gOut);
	}
}
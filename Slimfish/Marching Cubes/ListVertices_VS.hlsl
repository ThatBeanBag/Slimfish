
struct VSInput {
	uint z8_y8_x8_case8 : TEX;
};

struct VSOutput {
	uint z8_y8_x8_null5_edgeFlags3 : TEX;
};

VSOutput main( VSInput vIn )
{
	VSOutput vOut;

	// Get the cube case.
	int cubeCase = (int)(vIn.z8_y8_x8_case8 & 0xff);

	// Get the edge flags.
	int bit0 = (cubeCase) & 1;
	int bit3 = (cubeCase >> 3) & 1;
	int bit1 = (cubeCase >> 1) & 1;
	int bit4 = (cubeCase >> 4) & 1;
	int3 buildVertOnEdge = abs(int3(bit3, bit1, bit4) - bit0);

	// Set the edge flags.
	uint bits = vIn.z8_y8_x8_case8 & 0xffffff00;
	if (buildVertOnEdge.x != 0) {
		bits |= 1;
	}
	if (buildVertOnEdge.y != 0) {
		bits |= 2;
	}
	if (buildVertOnEdge.z != 0) {
		bits |= 4;
	}

	vOut.z8_y8_x8_null5_edgeFlags3 = bits;

	return vOut;
}
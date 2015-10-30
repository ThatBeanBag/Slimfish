
struct VSInput {
	uint z8_y8_x8_case8 : TEXCOORD;
};

struct VSOutput {
	uint z8_y8_x8_case8 : TEXCOORD;
};

VSOutput main(VSInput vIn) {
	// Just pass through.
	VSOutput vOut;
	vOut.z8_y8_x8_case8 = vIn.z8_y8_x8_case8;
	return vOut;
}

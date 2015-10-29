struct GSInput {
	uint z8_y8_x8_case8 : TEXCOORD;
};

struct GSOuput {
	uint z8_y8_x8_case8 : TEXCOORD;
};

[maxvertexcount(1)]
void main(point GSInput input[1], inout PointStream< GSOutput > output)
{
	// Get the cube case.
	uint cubeCase = GSInput.z8_y8_x8_case8 & 0xff;

	// Check if the cell is non-empty.
	if (cubeCase * (255 - cubeCase) > 0) {
		// List the cell as it's not empty.
		GSOutput output;
		ouput.z8_y8_x8_case8 = input[0].z8_y8_x8_case8;
		Stream.Append(output);
	}
}
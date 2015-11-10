struct GSInput {
	uint z8_y8_x8_case8 : TEX;
};

struct GSOutput {
	uint z8_y8_x8_case8 : TEX;
};

[maxvertexcount(1)]
void main(point GSInput gIn[1], inout PointStream< GSOutput > outputStream)
{
	// Get the cube case.
	uint cubeCase = gIn[0].z8_y8_x8_case8 & 0xff;

	// Check if the cell is non-empty.
	if (cubeCase * (255 - cubeCase) > 0) {
		// List the cell as it's not empty.
		GSOutput gOut;
		gOut.z8_y8_x8_case8 = gIn[0].z8_y8_x8_case8;
		outputStream.Append(gOut);
	}
}
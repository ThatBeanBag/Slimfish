cbuffer constantBuffer {
	float4x4 gWVP;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 colour : COLOR;
};

struct VSOutput
{
	float4 pos: SV_POSITION;
	float3 posW : POSITION;
};

// Convenience function for creating colours.
float4 CreateColour(float r, float g, float b, float a)
{
	return float4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

// Some useful constants.
static const float gPI = 3.14159265359f;

// Some colours.
static const float4 gWhite = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const float4 gBlack = float4(0.0f, 0.0f, 0.0f, 1.0f);
static const float4 gRed = float4(1.0f, 0.0f, 0.0f, 1.0f);
static const float4 gGreen = float4(0.0f, 1.0f, 0.0f, 1.0f);
static const float4 gBlue = float4(0.0f, 0.0f, 1.0f, 1.0f);
static const float4 gGold = CreateColour(255, 200, 0, 255);
static const float4 gGoldenYellow = CreateColour(255, 230, 0, 255);
static const float4 gDarkGold = CreateColour(200, 145, 12, 255);

float3 GetPointOnCircle(float3 origin, float radius, float angle)
{
	return float3(origin.x + (radius * cos(angle)),
				  origin.y + (radius * sin(angle)),
				  origin.z - 1.0f);
}

[maxvertexcount((gNumPoints * 6))]
void main(
	point VSOutput input[1], 
	inout TriangleStream< GSOutput > output
)
{
	float angleBetweenPoints = 2.0f * gPI / gNumPoints;
	float outerCircleRadius = tan(angleBetweenPoints) * sin(angleBetweenPoints);
	float3 centrePos = input[0].posW;

	float3 innerVerts[gNumPoints];
	float3 outerVerts[gNumPoints];

	// Inner circle of points.
	for (int i = 0; i < gNumPoints; ++i) {
		innerVerts[i] = GetPointOnCircle(centrePos, 1.0f, (i * angleBetweenPoints) - (gPI / 2.0f));
	}

	// Outer circle of points.
	for (int i = 0; i < gNumPoints; ++i) {
		outerVerts[i] = GetPointOnCircle(centrePos, outerCircleRadius, (i * angleBetweenPoints) + (gPI / 2.0f));
	}

	GSOutput centreOut = { mul(float4(centrePos, 1.0f), gWVP), gGold };

	for (int i = 0; i < gNumPoints; ++i) {
		GSOutput out1 = { mul(float4(outerVerts[i], 1.0f), gWVP), gGold };
		GSOutput out2 = { mul(float4(innerVerts[(i + 3) % gNumPoints], 1.0f), gWVP), gGold };

		output.Append(out1);
		output.Append(centreOut);
		output.Append(out2);
		output.RestartStrip();

		GSOutput out3 = { mul(float4(outerVerts[(i + 1) % gNumPoints], 1.0f), gWVP), gDarkGold };
		GSOutput out4 = { mul(float4(innerVerts[(i + 3) % gNumPoints], 1.0f), gWVP), gDarkGold };

		output.Append(out3);
		output.Append(out4);
		output.Append(centreOut);
		output.RestartStrip();
	}
}
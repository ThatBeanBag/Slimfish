cbuffer constantBuffer {
	float4x4 gWVP;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
};

struct VSOutput
{
	float4 pos: SV_POSITION;
	float3 posW : POSITION;
};

static const float gPI = 3.14159265359f;
static const float gOuterCircleRadius = 2.618f;
static const int gNumPoints = 5;

float3 getPointOnCircle(float3 origin, float radius, float angle)
{
	return float3(origin.x + (radius * cos(angle)),
				  origin.y + (radius * sin(angle)),
				  origin.z);
}

[maxvertexcount(gNumPoints * 2)]
void main(
	point VSOutput input[1], 
	inout TriangleStream< GSOutput > output
)
{
	float angleBetweenPoints = 2.0f * gPI / gNumPoints;
	float3 centrePos = input[0].posW;

	float3 innerVerts[gNumPoints];
	float3 outerVerts[gNumPoints];

	// Inner circle of points.
	for (int i = 0; i < gNumPoints; ++i) {
		innerVerts[i] = getPointOnCircle(centrePos, 1.0f, (i * angleBetweenPoints) - (gPI / 2.0f));
	}

	// Outer circle of points.
	for (int i = 0; i < gNumPoints; ++i) {
		outerVerts[i] = getPointOnCircle(centrePos, gOuterCircleRadius, (i * angleBetweenPoints) + (gPI / 2.0f));
	}

	for (int i = 0; i < gNumPoints; ++i) {
		GSOutput out1 = { mul(float4(outerVerts[i], 1.0f), gWVP) };
		GSOutput out3 = { mul(float4(input[0].pos.xyz, 1.0f), gWVP) };
		GSOutput out2 = { mul(float4(innerVerts[(i + 3) % gNumPoints], 1.0f), gWVP) };
		GSOutput out4 = { mul(float4(outerVerts[(i + 1) % gNumPoints], 1.0f), gWVP) };
		output.Append(out1);
		output.Append(out2);
		output.Append(out3);
		output.Append(out4);
		output.RestartStrip();

		output.Append(out4);
	}
}
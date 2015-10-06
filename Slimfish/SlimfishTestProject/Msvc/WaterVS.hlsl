
static const float PI = 3.1459f;

cbuffer constantBuffer {
	float4x4 gWorldMatrix;
	float4x4 gViewMatrix;
	float4x4 gProjectionMatrix;
	float4x4 gReflectionMatrix;
	float4x4 gWaveMatrix0;
	float4x4 gWaveMatrix1;
	float4x4 gWaveMatrix2;
	float4x4 gWaveMatrix3;
	float4x4 gLightViewMatrix;
	float4x4 gLightProjectionMatrix;
	float gElapsedTime;
};

static const int gNumWaves = 3;

struct VS_IN {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT {
	float4 position : SV_POSITION;
	float4 positionScreen : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 waveCoord0 : TEXCOORD1;
	float2 waveCoord1 : TEXCOORD2;
	float2 waveCoord2 : TEXCOORD3;
	float2 waveCoord3 : TEXCOORD4;
	float2 heightMapCoord : TEXCOORD5;
	float4 reflectionPosition : TEXCOORD6;
	float3 positionWorld : TEXCOORD7;
	float4 lightViewPosition : TEXCOORD8;
};

struct GerstnerWave {
	float steepness;
	float amplitude;
	float waveLength;
	float speed;
	float2 direction;
};

void SimulateWater(GerstnerWave waves[gNumWaves], float time, float3 position,
				   out float3 outPosition, out float3 outNormal, out float3 outTangent)
{
	outPosition = position;

	// Calculate positions.
	for (int i = 0; i < gNumWaves; ++i) {
		float A = waves[i].amplitude;						// Amplitude.
		float W = 2 * PI / waves[i].waveLength;				// Frequency.
		float P = waves[i].speed * W;						// Phase.
		float Q = waves[i].steepness / (W * A * gNumWaves);	// Steepness.
		float2 D = waves[i].direction;						// Direction.

		float innerTerm = W * dot(D, position.xz) + P * time;
		float cosOfTerm = cos(innerTerm);

		outPosition.x += Q * A * D.x * cosOfTerm;
		outPosition.y += A * sin(innerTerm);
		outPosition.z += Q * A * D.y * cosOfTerm;
	}

	// Calculate normals and tangents.
	float3 B = float3(0.0f, 0.0f, 0.0f);
	float3 T = float3(0.0f, 0.0f, 0.0f);
	float3 N = float3(0.0f, 0.0f, 0.0f);
	for (int j = 0; j < gNumWaves; ++j) {
		float A = waves[j].amplitude;						// Amplitude.
		float W = 2 * PI / waves[j].waveLength;				// Frequency.
		float P = waves[j].speed * W;						// Phase.
		float Q = waves[j].steepness / (W * A * gNumWaves);	// Steepness.
		float2 D = waves[j].direction;						// Direction.

		float WA = W * A;
		float innerTerm = W * dot(D, outPosition.xz) + P * time;
		float cosOfTerm = cos(innerTerm) /*/ 6.0f*/;
		float sinOfTerm = sin(innerTerm);
		B.x += Q * D.x * D.x * WA * sinOfTerm;
		B.y += Q * D.x * D.y * WA * sinOfTerm;
		B.z += D.x * WA * cosOfTerm;

		T.x += Q * D.x * D.y * WA * sinOfTerm;
		T.y += Q * D.y * D.y * WA * sinOfTerm;
		T.z += D.y * WA * cosOfTerm;

		N.x += D.x * WA * cosOfTerm;
		N.y += D.y * WA * cosOfTerm;
		N.z += Q * WA * sinOfTerm;
	}

	outTangent = normalize(float3(-T.x, 1.0f - T.y, T.z));
	outNormal = normalize(float3(-N.x, -N.y, 1.0f - N.z));
	//outNormal = normalize(float3(1 - B.x, -B.y, B.z));
}

float3 CreateGerstnerWave(float amplitude, float waveLength, float speed, float3 direction, float2 position,
						  out float3 normal,
						  out float3 tangent)
{
	float A = amplitude;
	float W = 2 * PI / waveLength;
	float P = speed * W;
	float Q = 1.0f / W * A;
	float3 D = direction;
	float T = gElapsedTime;

	float innerEquation = W * dot(D, float3(position.x, 0.0f, position.y)) + P * T;

	float3 gerstnerWave;
	gerstnerWave.x = Q * amplitude * D.x * cos(innerEquation);
	gerstnerWave.y = amplitude * sin(innerEquation);
	gerstnerWave.z = Q * amplitude * D.z * cos(innerEquation);

	float WA = W * A;
	float _S = sin(W * dot(D, gerstnerWave) + P * gElapsedTime);
	float _C = cos(W * dot(D, gerstnerWave) + P * gElapsedTime);

	normal.xz = D.xz * WA * _C;
	normal.y = Q * WA * _S;

	/*normal.x = Q * D.x * D.z * WA * _S;
	normal.y = D.z * WA * _C;
	normal.z = Q * D.z * D.z * WA * _S;*/

	/*normal.x = Q * D.x * D.x * WA * _S;
	normal.y = D.x * WA * _C;
	normal.z = Q * D.x * D.z * WA * _S;*/

	tangent = float3(1.0f, 0.0f, 0.0f);

	return gerstnerWave;
}

VS_OUT main(VS_IN vIn)
{
	VS_OUT vOut;

	/*//float gAmplitude = 0.4f;
	//float gWaveLength = 0.0002f;
	//float gSpeed = 0.00005f;
	//float3 gDirection1 = float3(-1.0f, 0.0, 1.0f);
	//float3 gDirection2 = float3(-1.0f, 0.0, -1.0f);
	//float3 gDirection3 = float3(-1.0f, 0.0, 1.0f);
	//float3 gDirection4 = float3(1.0f, 0.0,1.0f);

	//float amplitude = gAmplitude;
	//float frequency = 2 * PI / gWaveLength;
	//float phase = gSpeed * frequency;

	//vIn.position.y = amplitude * sin(dot(gDirection1, vIn.position) * frequency + gElapsedTime * phase);
	//vIn.position.y += amplitude * sin(dot(gDirection2, vIn.position) * frequency + gElapsedTime * phase);
	//vIn.position.y += amplitude * sin(dot(gDirection3, vIn.position) * frequency + gElapsedTime * phase);
	//vIn.position.y += amplitude * sin(dot(gDirection4, vIn.position) * frequency + gElapsedTime * phase);

	//float2 derivative = gDirection1 * frequency * amplitude * cos(dot(gDirection1, vIn.position) * frequency + gElapsedTime * phase);
	//float derivative2 = frequency * amplitude * cos(dot(direction2, vIn.position) * frequency + gElapsedTime * phase * 0.2f);
	//float derivativeX = direction.x * derivative + direction2.x * derivative2;
	//float derivativeY = direction.y * derivative + direction2.y * derivative2;

	//float3 tangent = normalize(float3(0, 1, derivative.y));
	//float3 normal = normalize(float3(-derivative.x, -derivative.y, 1));

	// Gerstner waves.
	float gAmplitude = 3.0f;
	float gWaveLength = 0.0002f;
	float gSpeed = 0.00005f;
	float3 gDirection1 = float3(-1.0f, 0.0f, 1.0f);

	float amplitude = gAmplitude;
	float frequency = 2 * PI / gWaveLength;
	float phase = gSpeed * frequency;
	float Q = 2.5f / frequency * amplitude;

	float innerEquation = frequency * dot(gDirection1.xz, vIn.position.xz) + phase * gElapsedTime;

	float3 gerstnerWave = vIn.position;
	float3 normal = float3(0.0f, 0.0f, 0.0f);
	float3 tangent = float3(0.0f, 0.0f, 0.0f);
	float3 normalTemp;
	float3 tangentTemp;
	//gerstnerWave += CreateGerstnerWave(10.0f, 0.0004f, 0.0001f, normalize(float3(1.0f, 0.0f, 0.0f)), vIn.position.xz, normalTemp, tangentTemp);
	normal += normalTemp;
	tangent += tangentTemp;
	//gerstnerWave += CreateGerstnerWave(2.0f, 0.0009f, 0.0001f, normalize(float3(1.0f, 0.0f, 1.0f)), vIn.position.xz, normalTemp, tangentTemp);
	normal += normalTemp;
	tangent += tangentTemp;
	gerstnerWave += CreateGerstnerWave(10.0f, 0.0004f, 0.0001f, normalize(float3(-1.0f, 0.0f, 0.21f)), vIn.position.xz, normalTemp, tangentTemp);
	normal += normalTemp;
	tangent += tangentTemp;
	//gerstnerWave += CreateGerstnerWave(6.0f, 0.00002f, 0.00001f, normalize(float3(-1.0f, 0.0f, -1.0f)), vIn.position.xz, normalTemp, tangentTemp);
	//gerstnerWave += CreateGerstnerWave(12.0f, 0.00006f, 0.000001f, normalize(float3(-1.0f, 0.0f, -0.4f)), vIn.position.xz, normalTemp, tangentTemp);
	//normal += normalTemp;
	//tangent += tangentTemp;

	normal.x = 1 - normal.x;
	normal.z = -normal.z;

	tangent.x = -tangent.x;
	tangent.z = 1 - tangent.y;

	normal = normalize(normal);
	tangent = normalize(tangent);

	normal = vIn.normal;
	tangent = vIn.tangent;

	vIn.position = gerstnerWave;*/

	GerstnerWave waves[gNumWaves] = { { 0.7f, 10.0f, 0.4f, 0.01f, normalize(float2(1.0f, 1.0f)) },
									  { 0.7f, 6.0f, 0.2f, 0.015f, normalize(float2(-1.0f, 0.4f)) },
							          { 0.7f, 2.0f, 0.08f, 0.02f, normalize(float2(1.0f, -0.6f)) } };
	float3 position;
	float3 normal;
	float3 tangent;
	SimulateWater(waves, gElapsedTime, vIn.position, position, normal, tangent);

	float4x4 worldViewProj = mul(mul(gWorldMatrix, gViewMatrix), gProjectionMatrix);
	float4x4 reflectProjWorld = mul(mul(gWorldMatrix, gReflectionMatrix), gProjectionMatrix);
	float4x4 lightViewProj = mul(mul(gWorldMatrix, gLightViewMatrix), gLightProjectionMatrix);

	vOut.position = mul(float4(position, 1.0f), worldViewProj);
	vOut.positionScreen = mul(float4(position, 1.0f), worldViewProj);
	vOut.positionWorld = mul(float4(position, 1.0f), gWorldMatrix).xyz;
	vOut.normal = mul(float4(normal, 0.0f), gWorldMatrix).xyz;
	vOut.tangent = mul(float4(tangent, 0.0f), gWorldMatrix).xyz;

	vOut.waveCoord0 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix0).xyzw;
	vOut.waveCoord1 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix1).xy;
	vOut.waveCoord2 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix2).xy;
	vOut.waveCoord3 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix3).xy;
	vOut.heightMapCoord = vIn.texCoord;
	vOut.reflectionPosition = mul(float4(position, 1.0f), reflectProjWorld);
	vOut.lightViewPosition = mul(float4(position, 1.0f), lightViewProj);

	return vOut;
}
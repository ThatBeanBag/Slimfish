
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

VS_OUT main(VS_IN vIn)
{
	VS_OUT vOut;

	//float gAmplitude = 0.4f;
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

	float4x4 worldViewProj = mul(mul(gWorldMatrix, gViewMatrix), gProjectionMatrix);
	float4x4 reflectProjWorld = mul(mul(gWorldMatrix, gReflectionMatrix), gProjectionMatrix);
	float4x4 lightViewProj = mul(mul(gWorldMatrix, gLightViewMatrix), gLightProjectionMatrix);

	vOut.position = mul(float4(vIn.position, 1.0f), worldViewProj);
	vOut.positionScreen = mul(float4(vIn.position, 1.0f), worldViewProj);
	vOut.positionWorld = mul(float4(vIn.position, 1.0f), gWorldMatrix).xyz;
	vOut.normal = mul(float4(vIn.normal, 0.0f), gWorldMatrix).xyz;
	vOut.tangent = mul(float4(vIn.tangent, 0.0f), gWorldMatrix).xyz;

	vOut.waveCoord0 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix0).xyzw;
	vOut.waveCoord1 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix1).xy;
	vOut.waveCoord2 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix2).xy;
	vOut.waveCoord3 = mul(float4(vIn.texCoord, 0.0f, 1.0f), gWaveMatrix3).xy;
	vOut.heightMapCoord = vIn.texCoord;
	vOut.reflectionPosition = mul(float4(vIn.position, 1.0f), reflectProjWorld);
	vOut.lightViewPosition = mul(float4(vIn.position, 1.0f), lightViewProj);

	return vOut;
}
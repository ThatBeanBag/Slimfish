
float3 rotate(float3 coord, float4x4 transform)
{
	return float3(dot(coord, transform._11_12_13),
				  dot(coord, transform._21_22_23),
				  dot(coord, transform._31_32_33));
}

// Sampling functions
const static float gNOISE_VOLUME_DIM = 16;
const static float gINV_NOISE_VOLUME_DIM = 1.0f / gNOISE_VOLUME_DIM;

float4 SampleNoiseLQU(float3 uvw, Texture3D noiseVolume) 
{
	return noiseVolume.SampleLevel(gSamplerTrilinearWrap, uvw, 0);
}

float4 SampleNoiseLQS(float3 uvw, Texture3D noiseVolume)
{
	return SampleNoiseLQU(uvw, noiseVolume) * 2 - 1.0f;
}

float4 SampleNoiseMQU(float3 uvw, Texture3D noiseVolume)
{
	// Smooth the input coordinate.
	float3 t = frac(uvw * gNOISE_VOLUME_DIM + 0.5f);
	float3 t2 = (3 - 2 * t) * t * t;
	float3 uvw2 = uvw + (t2 - t) / gNOISE_VOLUME_DIM;

	return SampleNoiseLQU(uvw2, noiseVolume);
}

float4 SampleNoiseMQS(float3 uvw, Texture3D noiseVolume)
{
	return SampleNoiseMQU(uvw, noiseVolume) * 2 - 1.0f;
}

float SampleNoiseHQU(float3 uvw, Texture3D noiseVolume, float smooth = 1)
{
	float3 uvw2 = floor(uvw * gNOISE_VOLUME_DIM) * gINV_NOISE_VOLUME_DIM;
	float3 t = (uvw - uvw2) * gNOISE_VOLUME_DIM;
	t = lerp(t, t * t * (3 - 2 * t), smooth);

	float2 invDimAndZero = float2(gINV_NOISE_VOLUME_DIM, 0.0f);

	float4 f1 = noiseVolume.SampleLevel(gSamplerTrilinearWrap, uvw2, 0).zxyw; // <+0, +y, +z, +yz>
	float4 f2 = noiseVolume.SampleLevel(gSamplerTrilinearWrap, uvw2 + invDimAndZero.xyy, 0).zxyw; // <+x, +xy, +xz, +xyz>
	float4 f3 = lerp(f1, f2, t.xxxx);  // f3 = <+0, +y, +z, +yz>
	float2 f4 = lerp(f3.xy, f3.zw, t.yy); // f4 = <+0, +z>
	float  f5 = lerp(f4.x, f4.y, t.z);

	return f5;
}

float SampleNoiseHQS(float3 uvw, Texture3D noiseVolume, float smooth = 1)
{
	return SampleNoiseHQU(uvw, noiseVolume) * 2 - 1.0f;
}

const static float gPRE_WARP_STR = 35;

float SmoothSnap(float t, float m)
{
	float c = (t > 0.5) ? 1 : 0;
	float s = 1 - c * 2;
	return c + s*pow((c + s*t) * 2, m)*0.5;
}

/** Calculate the density for a coordinate in world-space.
	@remarks
		This is the main function for the marching cubes algorithm it
		determines the shape of the terrain.
	@param 
		wsCoord The coordinate in world-space to calculate the density 
		value for.
*/
float Density(float3 wsCoord) {

	// Take a copy of the world-space coordinate.
	float3 wsCoordCopy = wsCoord;

	//float maxDistance = 0.2f;
	//float density = 1.0f - (length(wsCoord) / maxDistance);

	// The density value of this world-space coordinate to calculate.
	float density = (-wsCoord.x + -wsCoord.y) / 2.0f;
	density = -wsCoord.y;
	//density = wsCoord.z;

	density += sin(wsCoord.x * 1.0f);

	float4 uulf_rand1 = saturate(SampleNoiseMQU(wsCoord * 0.000718, gTexture3DNoise0) * 2 - 0.5f);
	float4 uulf_rand2 = SampleNoiseMQU(wsCoord * 0.000632, gTexture3DNoise1);
	float4 uulf_rand3 = SampleNoiseMQU(wsCoord * 0.000695, gTexture3DNoise2);

	// Pre-warp the world-space coordinate.
	float3 ulf_rand = SampleNoiseMQS(wsCoord * 0.0041, gTexture3DNoise2).xyz * 0.64f
				    + SampleNoiseMQS(wsCoord * 0.0041 * 0.427, gTexture3DNoise3).xyz * 0.32f;

	wsCoord += ulf_rand * gPRE_WARP_STR * saturate(uulf_rand3.x * 1.4f - 0.3f);

	// Crusty shelf.
	float shelf_thickness_y = 2.5;//2.5;
	float shelf_pos_y = -1;//-2;
	float shelf_strength = 9.5;   // 1-4 is good
	density = lerp(density, shelf_strength, 0.83*saturate(shelf_thickness_y - abs(wsCoord.y - shelf_pos_y)) * saturate(uulf_rand1.y*1.5 - 0.5));

	// Flat terracing.
	const float terraces_can_warp = 0.5 * uulf_rand2.y;
	const float terrace_freq_y = 0.13;
	const float terrace_str = 3 * saturate(uulf_rand1.z * 2 - 1); 
	const float overhang_str = 1 * saturate(uulf_rand1.z * 2 - 1);
	float fy = -lerp(wsCoordCopy.y, wsCoord.y, terraces_can_warp)*terrace_freq_y;
	float orig_t = frac(fy);
	float t = orig_t;
	t = SmoothSnap(t, 16);  // faster than using 't = t*t*(3-2*t)' four times
	fy = floor(fy) + t;
	density += fy*terrace_str;
	density += (t - orig_t) * overhang_str;

	// Repeating ridges on [warped] Y coord:
	density += SampleNoiseLQS(wsCoord.xyz*float3(2, 27, 2)*0.0037, gTexture3DNoise0).x * 2 * saturate(uulf_rand2.w * 2 - 1);
	//density = -wsCoordCopy.y;

	// Sample 9 octaves
	density += SampleNoiseLQS(wsCoordCopy*0.3200*0.934, gTexture3DNoise3).x*0.16*1.20; // skipped for long-range ambo
	density += SampleNoiseLQS(wsCoordCopy*0.1600*1.021, gTexture3DNoise1).x*0.32*1.16;  // skipped for long-range ambo
	density += SampleNoiseLQS(wsCoordCopy*0.0800*0.985, gTexture3DNoise2).x*0.64*1.12;  // skipped for long-range ambo
	density += SampleNoiseLQS(wsCoordCopy*0.0400*1.051, gTexture3DNoise0).x*1.28*1.08;  // skipped for long-range ambo
	density += SampleNoiseLQS(wsCoordCopy*0.0200*1.020, gTexture3DNoise1).x*2.56*1.04;
	density += SampleNoiseLQS(wsCoordCopy*0.0100*0.968, gTexture3DNoise3).x * 2;
	density += SampleNoiseMQS(wsCoordCopy*0.0050*0.994, gTexture3DNoise0).x * 10 * 1.0; // MQ
	density += SampleNoiseMQS(wsCoordCopy*0.0025*1.045, gTexture3DNoise2).x * 20 * 0.9; // MQ
	//density += NHQs(c7*0.0012*0.972, packedNoiseVol3).x * 40 * 0.8 // HQ and *rotated*!

	//density += gTexture3DNoise0.SampleLevel(gSamplerTrilinearWrap, wsCoord * 2.0f).x * 0.5f;
	//density += gTexture3DNoise1.Sample(gSamplerTrilinearWrap, wsCoord * 4.03).x * 0.25f;
	//density += gTexture3DNoise2.Sample(gSamplerTrilinearWrap, wsCoord * 1.96).x * 0.5f;
	//density += gTexture3DNoise3.Sample(gSamplerTrilinearWrap, wsCoord * 1.01).x * 1.0f;
	//density += 0.34f;

	density -= gWChunkSize * 0.0000001f;

	return density;
}
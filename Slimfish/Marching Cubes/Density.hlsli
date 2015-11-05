
float3 rotate(float3 coord, float4x4 transform)
{
	return float3(dot(coord, transform._11_12_13),
				  dot(coord, transform._21_22_23),
				  dot(coord, transform._31_32_33));
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

	//density += gTexture3DNoise0.Sample(gSamplerTrilinearWrap, wsCoord * 2.0f).x * 10.0f;
	//density += gTexture3DNoise1.Sample(gSamplerTrilinearWrap, wsCoord * 4.03).x * 0.25f;
	//density += gTexture3DNoise2.Sample(gSamplerTrilinearWrap, wsCoord * 1.96).x * 0.5f;
	//density += gTexture3DNoise3.Sample(gSamplerTrilinearWrap, wsCoord * 1.01).x * 1.0f;
	//density += 0.34f;

	density -= gWChunkSize * 0.0000001f;

	return density;
}
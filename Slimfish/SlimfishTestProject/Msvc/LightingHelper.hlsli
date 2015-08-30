
struct Light {
	int m_Type;
	float4 m_Diffuse;
	float4 m_Specular;
	float3 m_Position;
	float3 m_Direction;
	float m_Range;
	float m_Falloff;
	float3 m_Attenuation;
	float m_SpotTheta;
	float m_SpotPhi;
};

struct SurfaceInfo
{
	float3 m_Position;
	float3 m_Normal;
	float4 m_Diffuse;
	float4 m_Specular;
};

float3 ParallelLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

		// The light vector aims opposite the direction the light rays travel.
		float3 lightVec = -L.m_Direction;

		// Add the ambient term.
		litColor += v.m_Diffuse.xyz;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, v.m_Normal);
	[branch]
	if (diffuseFactor > 0.0f)
	{
		float specPower = max(v.m_Specular.a, 1.0f);
		float3 toEye = normalize(eyePos - v.m_Position);
			float3 R = reflect(-lightVec, v.m_Normal);
			float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * v.m_Diffuse.xyz * L.m_Diffuse.xyz;
		litColor += specFactor * v.m_Specular.xyz * L.m_Specular.xyz;
	}

	return litColor;
}

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

		// The vector from the surface to the light.
		float3 lightVec = L.m_Position - v.m_Position;

		// The distance from surface to light.
		float d = length(lightVec);

	if (d > L.m_Range)
		return float3(0.0f, 0.0f, 0.0f);

	// Normalize the light vector.
	lightVec /= d;

	// Add the ambient light term.
	litColor += v.m_Diffuse.xyz;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, v.m_Normal);
	[branch]
	if (diffuseFactor > 0.0f)
	{
		float specPower = max(v.m_Specular.a, 1.0f);
		float3 toEye = normalize(eyePos - v.m_Position);
			float3 R = reflect(-lightVec, v.m_Normal);
			float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * v.m_Diffuse.xyz * L.m_Diffuse.xyz;
		litColor += specFactor * v.m_Specular.xyz * L.m_Specular.xyz;
	}

	// attenuate
	return litColor / dot(L.m_Attenuation, float3(1.0f, d, d*d));
}

float3 SpotLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = PointLight(v, L, eyePos);

		// The vector from the surface to the light.
		float3 lightVec = normalize(L.m_Position - v.m_Position);

		float s = pow(max(dot(-lightVec, L.m_Direction), 0.0f), L.m_SpotTheta);

	// Scale color by spotlight factor.
	return litColor*s;
}




struct Light {
	int type;
	float4 diffuse;
	float4 specular;
	float3 position;
	float3 direction;
	float range;
	float falloff;
	float3 attenuation;
	float spotTheta;
	float spotPhi;
};

struct Material {
	float4 diffuse;
	float4 specular;
	float4 emmisive;
	float specularPower;
};

float3 ParallelLight(float3 normal, Material material, Light light, float3 toEye)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -light.direction;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);
	[branch]
	if (diffuseFactor > 0.0f) {
		float3 R = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), material.specularPower);

		// diffuse and specular terms
		litColor += diffuseFactor * material.diffuse.xyz * light.diffuse.xyz;
		litColor += specFactor * material.specular.xyz * light.specular.xyz;
	}

	return litColor.xyz;
}

float3 PointLight(float3 position, float3 normal, Material material, Light light, float3 toEye)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = light.position - position;

	// The distance from surface to light.
	float distanceToLight = length(lightVec);

	if (distanceToLight > light.range) {
		return float3(0.0f, 0.0f, 0.0f);
	}

	// Normalize the light vector.
	lightVec /= distanceToLight;

	// Add the ambient light term.
	litColor += material.diffuse.xyz;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);
	[branch]
	if (diffuseFactor > 0.0f) {
		float3 R = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), material.specularPower);

		// diffuse and specular terms
		litColor += diffuseFactor * material.diffuse.xyz * material.diffuse.xyz;
		litColor += specFactor * material.specular.xyz * material.specular.xyz;
	}

	// Attenuate
	return litColor / dot(light.attenuation, float3(1.0f, distanceToLight, distanceToLight * distanceToLight));
}

float3 SpotLight(float3 position, float3 normal, Material material, Light light, float3 toEye)
{
	float3 litColor = PointLight(position, normal, material, light, toEye);

	// The vector from the surface to the light.
	float3 lightVec = normalize(light.position - position);

	float s = pow(max(dot(-lightVec, light.direction), 0.0f), light.spotTheta);

	// Scale color by spotlight factor.
	return litColor * s;
}



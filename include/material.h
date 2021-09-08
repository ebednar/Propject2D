#pragma once

class Material
{
public:
	/*float	diffuse[3];
	float	specular[3];
	int		shininess;*/
	float albedo[3] = { 0.0f, 0.0f, 0.0f };
	float ao = 1.0f;
	float metallic = 0.9f;
	float roughness = 0.2f; // 0.05f..1.0f
public:
	void load_material(char* path);
	void load_material(float albedo[3], float ao, float metallic, float roughness);
};


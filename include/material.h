#pragma once

class Material
{
public:
	float			diffuse[3];
	float			specular[3];
	int				shininess;
	int				shader_id;
	unsigned int	texture_id;
	int				text_width;
	int				text_height;
public:
	void	load_material(char* path);
	void	load_material(float albedo[3], float ao, float metallic, float roughness);
	void	set_shader(int shader);
	void	set_texture(unsigned int texture);
};

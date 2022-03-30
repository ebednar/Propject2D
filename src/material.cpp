#include <iostream>
#include <fstream>
#include <sstream>

#include "material.h"

void Material::load_material(float albedo[3], float ao, float metallic, float roughness)
{

}

void Material::load_material(char* path)
{

}

void Material::set_shader(int shader)
{
	shader_id = shader;
}

void Material::set_texture(unsigned int texture, int width, int height)
{
	texture_id = texture;
	text_width = width;
	text_height = height;
}

void Material::set_texture(unsigned int texture)
{
	texture_id = texture;
}
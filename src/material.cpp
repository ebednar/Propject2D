#include <iostream>
#include <fstream>
#include <sstream>

#include "material.h"

void Material::load_material(float albedo[3], float ao, float metallic, float roughness)
{
	this->albedo[0] = albedo[0];
	this->albedo[1] = albedo[1];
	this->albedo[2] = albedo[2];
	this->ao = ao;
	this->metallic = metallic;
	this->roughness = roughness;

}

void Material::load_material(char* path)
{

}
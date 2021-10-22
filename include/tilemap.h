#pragma once
#include "model.h"

struct Tile
{
	int id = 0;
	int	x = 0;
	int y = 0;;
};

class Tilemap
{
public:
	Model			mod;
	float*			vertices;
	unsigned int	map_numb = 0;
	int				shader_id = 0;
	unsigned int	texture_id = 0;
	Tile*			tiles = nullptr;
public:
	void	generate_batched_map(Model* mod);
};
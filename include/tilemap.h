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
	Model*			mod = new Model();
	float*			vertices;
	unsigned int	map_numb = 0;
	int				shader_id;
	unsigned int	texture_id;
	Tile*			tiles;
public:
	void	generate_batched_map(Model* mod);
};
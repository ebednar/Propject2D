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
	Tile*			tiles = nullptr;
	float*			vertices;
	unsigned int	tile_numb = 0;
	unsigned int	raws = 0;
	unsigned int	columns = 0;
	unsigned int	texture_id = 0;
	int				shader_id = 0;
public:
	void	generate_batched_map(Model* mod);
};
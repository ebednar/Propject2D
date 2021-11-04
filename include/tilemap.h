#pragma once
#include "model.h"
#include <string>

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
	int				raws = 0;
	int				columns = 0;
	unsigned int	texture_id = 0;
	int				shader_id = 0;
public:
	void	generate_batched_map(Model* sprite_mod);
	void	regenerate_new_map(Model* sprite_mod, int new_raws, int new_columns);
	void	regenerate_map(Model* sprite_mod);
	void	save_tilemap(std::string name);
};
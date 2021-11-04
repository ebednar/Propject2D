#include "tilemap.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

void	Tilemap::generate_batched_map(Model* sprite_mod)
{
	vertices = new float[sprite_mod->ind_number * 8 * tile_numb];
	for (int i = 0; i < tile_numb; ++i)
	{
		for (int j = 0; j < sprite_mod->ind_number; ++j)
		{
			vertices[i * 48 + j * 8] = sprite_mod->vertices[j * 8] + tiles[i].x; // position.x
			vertices[i * 48 + j * 8 + 1] = sprite_mod->vertices[j * 8 + 1] - tiles[i].y; // position.y
			vertices[i * 48 + j * 8 + 2] = sprite_mod->vertices[j * 8 + 2]; // position.z
			vertices[i * 48 + j * 8 + 3] = sprite_mod->vertices[j * 8 + 3] * 0.05 + 0.0015 + 7 * 0.05; // uv.x    // 20  320
			vertices[i * 48 + j * 8 + 4] = sprite_mod->vertices[j * 8 + 4] * 0.125 + 0.005 + tiles[i].id * 0.125; // uv.y    // 7  128
			vertices[i * 48 + j * 8 + 5] = sprite_mod->vertices[j * 8 + 5]; // normal.x
			vertices[i * 48 + j * 8 + 6] = sprite_mod->vertices[j * 8 + 6]; // normal.y
			vertices[i * 48 + j * 8 + 7] = sprite_mod->vertices[j * 8 + 7]; // normal.z
		}
	}
	mod.vertices = vertices;
	mod.ind_number = sprite_mod->ind_number * tile_numb;
	mod.vertex_buffer();
}

void	Tilemap::regenerate_new_map(Model* sprite_mod, int new_raws, int new_columns)
{
	//delete vertices;

	Tile* new_tiles = new Tile[new_raws * new_columns];

	for (int i = 0; i < new_raws; ++i)
	{
		for (int j = 0; j < new_columns; ++j)
		{
			new_tiles[i * new_columns + j].id = 0;
			new_tiles[i * new_columns + j].x = j;
			new_tiles[i * new_columns + j].y = i;
		}
	}

	int raws_tmp = raws > new_raws ? new_raws : raws;
	int columns_tmp = columns > new_columns ? new_columns : columns;
	for (int i = 0; i < raws_tmp; ++i)
	{
		for (int j = 0; j < columns_tmp; ++j)
		{
			new_tiles[i * new_columns + j].id = tiles[i * columns + j].id;
		}
	}

	delete[] vertices;
	delete[] tiles;
	tiles = new_tiles;
	raws = new_raws;
	columns = new_columns;
	tile_numb = raws * columns;
	generate_batched_map(sprite_mod);
}

void	Tilemap::regenerate_map(Model* sprite_mod)
{

	for (int i = 0; i < tile_numb; ++i)
	{
		for (int j = 0; j < sprite_mod->ind_number; ++j)
		{
			vertices[i * 48 + j * 8 + 3] = sprite_mod->vertices[j * 8 + 3] * 0.05 + 0.0015 + 7 * 0.05; // uv.x    // 20  320
			vertices[i * 48 + j * 8 + 4] = sprite_mod->vertices[j * 8 + 4] * 0.125 + 0.005 + tiles[i].id * 0.125; // uv.y    // 7  128
		}
	}
	mod.vertex_buffer();
}

void	Tilemap::save_tilemap(std::string name)
{
	std::stringstream out;
	out << raws << '\n';
	out << columns << '\n';
	int j = 0;
	for (int i = 0; i < raws; ++i)
	{
		for (j = 0; j < columns - 1; ++j)
		{
			out << tiles[i * columns + j].id << ' ';
		}
		out << tiles[i * columns + j].id << '\n';
	}
	std::ofstream	file("res/scene/" + name);
	file << out.str();
	file.close();
}
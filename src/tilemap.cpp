#include "tilemap.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

void	Tilemap::generate_batched_map(Model* sprite_mod)
{
	vertices = new Vertex[sprite_mod->ind_number * tile_numb];
	for (int i = 0; i < tile_numb; ++i)
	{
		for (int j = 0; j < sprite_mod->ind_number; ++j)
		{
			vertices[i * 6 + j].pos[0] = sprite_mod->vertices[j].pos[0] + tiles[i].x; // position.x
			vertices[i * 6 + j].pos[1] = sprite_mod->vertices[j].pos[1] - tiles[i].y; // position.y
			vertices[i * 6 + j].pos[2] = sprite_mod->vertices[j].pos[2]; // position.z
			//vertices[i * 6 + j].uv[0] = sprite_mod->vertices[j].uv[0] * 0.05 + 0.0015 + 7 * 0.05; // uv.x    // 20  320
			//vertices[i * 6 + j].uv[1] = sprite_mod->vertices[j ].uv[1] * 0.125 + 0.005 + tiles[i].id * 0.125; // uv.y    // 7  128
			if (sprite_mod->vertices[j].uv[0] == 0.0f)
				vertices[i * 6 + j].uv[0] = (sprite_mod->vertices[j].uv[0] + tiles[i].idx + 0.005) * 1.0 / (256 / 32); // uv.x
			else
				vertices[i * 6 + j].uv[0] = (sprite_mod->vertices[j].uv[0] + tiles[i].idx - 0.005) * 1.0 / (256 / 32); // uv.x
			if (sprite_mod->vertices[j].uv[1] == 0.0f)
				vertices[i * 6 + j].uv[1] = (sprite_mod->vertices[j].uv[1] + tiles[i].idy + 0.000) * 1.0 / (4256 / 32) ; // uv.y
			else
				vertices[i * 6 + j].uv[1] = (sprite_mod->vertices[j].uv[1] + tiles[i].idy - 0.000) * 1.0 / (4256 / 32) ; // uv.y
			vertices[i * 6 + j].normal[0] = sprite_mod->vertices[j ].normal[0]; // normal.x
			vertices[i * 6 + j].normal[1] = sprite_mod->vertices[j ].normal[1]; // normal.y
			vertices[i * 6 + j].normal[2] = sprite_mod->vertices[j ].normal[2]; // normal.z
			vertices[i * 6 + j].id = i;
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
			new_tiles[i * new_columns + j].idx = 0;
			new_tiles[i * new_columns + j].idy = 0;
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
			new_tiles[i * new_columns + j].idx = tiles[i * columns + j].idx;
			new_tiles[i * new_columns + j].idy = tiles[i * columns + j].idy;
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
			//vertices[i * 6 + j].uv[0] = sprite_mod->vertices[j].uv[0] * 0.05 + 0.0015 + 7 * 0.05; // uv.x    // 20  320
			//vertices[i * 6 + j].uv[1] = sprite_mod->vertices[j].uv[1] * 0.125 + 0.005 + tiles[i].id * 0.125; // uv.y    // 7  128
			if (sprite_mod->vertices[j].uv[0] == 0.0f)
				vertices[i * 6 + j].uv[0] = (sprite_mod->vertices[j].uv[0] + tiles[i].idx + 0.005) * 1.0 / (256 / 32); // uv.x
			else
				vertices[i * 6 + j].uv[0] = (sprite_mod->vertices[j].uv[0] + tiles[i].idx - 0.005) * 1.0 / (256 / 32); // uv.x
			if (sprite_mod->vertices[j].uv[1] == 0.0f)
				vertices[i * 6 + j].uv[1] = (sprite_mod->vertices[j].uv[1] + tiles[i].idy + 0.01) * 1.0 / (4256 / 32); // uv.y
			else
				vertices[i * 6 + j].uv[1] = (sprite_mod->vertices[j].uv[1] + tiles[i].idy - 0.01) * 1.0 / (4256 / 32); // uv.y
		}
	}
	mod.update_vbo();
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
			out << tiles[i * columns + j].idx << '/' << tiles[i * columns + j].idy << ' ';
		}
		out << tiles[i * columns + j].idx << '/' << tiles[i * columns + j].idy << '\n';
	}
	std::ofstream	file("res/scene/" + name);
	file << out.str();
	file.close();
}
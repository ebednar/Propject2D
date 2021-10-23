#include "tilemap.h"
#include <iostream>
#include <map>
//float arr[] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

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
			vertices[i * 48 + j * 8 + 4] = sprite_mod->vertices[j * 8 + 4] * 0.125 + 0.005 + tiles[i].id * 0.125; // uv.z    // 7  128
			vertices[i * 48 + j * 8 + 5] = sprite_mod->vertices[j * 8 + 5]; // normal.x
			vertices[i * 48 + j * 8 + 6] = sprite_mod->vertices[j * 8 + 6]; // normal.y
			vertices[i * 48 + j * 8 + 7] = sprite_mod->vertices[j * 8 + 7]; // normal.z
		}
	}
	mod.vertices = vertices;
	mod.ind_number = sprite_mod->ind_number * tile_numb;
	mod.vertex_buffer();
	delete[] vertices;
	vertices = nullptr;
}
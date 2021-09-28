#include "tilemap.h"
#include <iostream>

//float arr[] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

void	Tilemap::generate_batched_map(Model* sprite_mod)
{
	vertices = new float[sprite_mod->ind_number * 8 * map_numb];
	for (int i = 0; i < map_numb; ++i)
	{
		for (int j = 0; j < sprite_mod->ind_number; ++j)
		{
			vertices[i * 48 + j * 8] = sprite_mod->vertices[j * 8] + tiles[i].x; // position.x
			vertices[i * 48 + j * 8 + 1] = sprite_mod->vertices[j * 8 + 1] - tiles[i].y; // position.y
			vertices[i * 48 + j * 8 + 2] = sprite_mod->vertices[j * 8 + 2]; // position.z
			vertices[i * 48 + j * 8 + 3] = sprite_mod->vertices[j * 8 + 3]; // uv.x
			vertices[i * 48 + j * 8 + 4] = sprite_mod->vertices[j * 8 + 4]; // uv.z
			vertices[i * 48 + j * 8 + 5] = sprite_mod->vertices[j * 8 + 5]; // normal.x
			vertices[i * 48 + j * 8 + 6] = sprite_mod->vertices[j * 8 + 6]; // normal.y
			vertices[i * 48 + j * 8 + 7] = sprite_mod->vertices[j * 8 + 7]; // normal.z
		}
	}
	//for (int i = 0; i < map_numb; ++i)
	//{
	//	for (int j = 0; j < sprite_mod->ind_number; ++j)
	//	{
	//		std::cout << vertices[i * 48 + j * 8] << " " << vertices[i * 48 + j * 8 + 1] << " " << vertices[i * 48 + j * 8 + 2] << " " << vertices[i * 48 + j * 8 + 3] << " "
	//			<< vertices[i * 48 + j * 8 + 4] << " " << vertices[i * 48 + j * 8 + 5] << " " << vertices[i * 48 + j * 8 + 6] << " " << vertices[i * 48 + j * 8 + 7] << " " << std::endl;
	//	}
	//}
	mod->vertices = vertices;
	mod->ind_number = sprite_mod->ind_number * map_numb;
	mod->vertex_buffer();
}
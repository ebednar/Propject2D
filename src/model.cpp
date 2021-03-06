#include "model.h"
#include "glad.h"
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>

int Model::load_obj(const char* path, bool loadBuf)
{
	std::ifstream		file;

	file.open(path);
	if (not file.is_open())
	{
		std::cout << "model file not found" << std::endl;
		return (1);
	}
	std::string line;
	std::vector< unsigned int > vertex_ind, uv_ind, normal_ind;
	std::vector< glm::vec3 > vec_vertices;
	std::vector< glm::vec2 > vec_uvs;
	std::vector< glm::vec3 > vec_normals;
	vert_number = 0;
	ind_number = 0;

	while (std::getline(file, line))
	{
		const char* str = line.c_str();
		if (str[0] == 'v' && str[1] == ' ')
		{
			vert_number++;
			glm::vec3 vert;
			if (sscanf(str + 2, "%f %f %f", &vert.x, &vert.y, &vert.z) != 3)
			{
				std::cout << "model file corrupted" << std::endl;
				return(1);
			}
			vec_vertices.push_back(vert);
		}
		if (str[0] == 'v' && str[1] == 't')
		{
			glm::vec2 vert;
			if (sscanf(str + 2, "%f %f", &vert.x, &vert.y) != 2)
			{
				std::cout << "model file corrupted" << std::endl;
				return(1);
			}
			vec_uvs.push_back(vert);
		}
		if (str[0] == 'v' && str[1] == 'n')
		{
			glm::vec3 vert;
			if (sscanf(str + 2, "%f %f %f", &vert.x, &vert.y, &vert.z) != 3)
			{
				std::cout << "model file corrupted" << std::endl;
				return(1);
			}
			vec_normals.push_back(vert);
		}
		if (str[0] == 'f')
		{
			const char* ptr1;
			int		count;
			count = 0;
			ptr1 = str + 1;
			while (*ptr1 != '\0')
				if (*ptr1 >= '0' && *ptr1 <= '9')
				{
					while ((*ptr1 >= '0' && *ptr1 <= '9') || *ptr1 == '/')
					{
						ptr1++;
					}
					count++;
				}
				else
					ptr1++;

			if (count == 3)
			{
				unsigned int ivert[3];
				unsigned int itext[3];
				unsigned int inorm[3];
				int m = sscanf(str + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &ivert[0], &itext[0], &inorm[0], &ivert[1], &itext[1], &inorm[1], &ivert[2], &itext[2], &inorm[2]);
				if (m == 9)
				{
					vertex_ind.push_back(ivert[0]);
					vertex_ind.push_back(ivert[1]);
					vertex_ind.push_back(ivert[2]);
					uv_ind.push_back(itext[0]);
					uv_ind.push_back(itext[1]);
					uv_ind.push_back(itext[2]);
					normal_ind.push_back(inorm[0]);
					normal_ind.push_back(inorm[1]);
					normal_ind.push_back(inorm[2]);
				}
				else
				{
					std::cout << "model file corrupted" << std::endl;
					return(1);
				}
			}
			else
			{

				unsigned int ivert[4];
				unsigned int itext[4];
				unsigned int inorm[4];

				int m = sscanf(str + 2, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &ivert[0], &itext[0], &inorm[0], &ivert[1], &itext[1], &inorm[1], &ivert[2], &itext[2], &inorm[2], &ivert[3], &itext[3], &inorm[3]);
				if (m == 12)
				{
					vertex_ind.push_back(ivert[0]);
					vertex_ind.push_back(ivert[1]);
					vertex_ind.push_back(ivert[2]);
					vertex_ind.push_back(ivert[2]);
					vertex_ind.push_back(ivert[3]);
					vertex_ind.push_back(ivert[0]);
					uv_ind.push_back(itext[0]);
					uv_ind.push_back(itext[1]);
					uv_ind.push_back(itext[2]);
					uv_ind.push_back(itext[2]);
					uv_ind.push_back(itext[3]);
					uv_ind.push_back(itext[0]);
					normal_ind.push_back(inorm[0]);
					normal_ind.push_back(inorm[1]);
					normal_ind.push_back(inorm[2]);
					normal_ind.push_back(inorm[2]);
					normal_ind.push_back(inorm[3]);
					normal_ind.push_back(inorm[0]);
				}
				else
				{
					std::cout << "model file corrupted" << std::endl;
					return(1);
				}
			}
		}
	}
	file.close();
	ind_number = vertex_ind.size();
	vertices = new Vertex[vertex_ind.size()];
	for (unsigned int i = 0; i < vertex_ind.size(); ++i)
	{
		vertices[i].pos[0] = vec_vertices[vertex_ind[i] - 1].x;
		vertices[i].pos[1] = vec_vertices[vertex_ind[i] - 1].y;
		vertices[i].pos[2] = vec_vertices[vertex_ind[i] - 1].z;
		vertices[i].uv[0] = vec_uvs[uv_ind[i] - 1].x;
		vertices[i].uv[1] = vec_uvs[uv_ind[i] - 1].y;
		vertices[i].normal[0] = vec_normals[normal_ind[i] - 1].x;
		vertices[i].normal[1] = vec_normals[normal_ind[i] - 1].y;
		vertices[i].normal[2] = vec_normals[normal_ind[i] - 1].z;
		vertices[i].id = 4;
	}
	if (loadBuf)
		vertex_buffer();

	return 0;
}

void Model::vertex_buffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ibo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * ind_number * sizeof(float), vertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_number * sizeof(float), indices, GL_STATIC_DRAW);*/

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// entity id
	glVertexAttribIPointer(3, 1, GL_INT, 9 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::update_vbo()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * ind_number * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

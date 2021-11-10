#pragma once

struct Vertex
{
	float	pos[3];
	float	uv[2];
	float	normal[3];
	int		id;
};

class Model
{
public:
	Vertex			*vertices;
	unsigned int	*indices;
	unsigned int	vert_number;
	unsigned int	ind_number;
	unsigned int	vbo;
	unsigned int	vao;
	unsigned int	ibo;
public:
	int		load_obj(const char *path, bool loadBuf);
	void	vertex_buffer();
};

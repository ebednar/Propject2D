#pragma once

struct Vertex
{
	float	pos[3];
	float	uv[2];
	float	normal[3];
	int		id = -1;
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
	Model() = default;
	Model(Model& mod) = default;
	int		load_obj(const char *path, bool loadBuf);
	void	vertex_buffer();
	void	update_vbo();
};

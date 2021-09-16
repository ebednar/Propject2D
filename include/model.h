#pragma once

class Model
{
public:
	float			*vertices;
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

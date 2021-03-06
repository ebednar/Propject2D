#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <map>
#include <string>

enum Slot
{
    Dialog,
    NearBackgrong,
    FarBackground
};

struct vec2
{
    float x;
    float y;
};

struct text_t
{
    std::string str;
    float       x;
    float       y;
    float       scale;
    bool        enable = false;
    text_t(std::string str = " ", float x = 0.0f, float y = 0.0f, float scale = 1.0f) : str(str), x(x), y(y), scale(scale) {}
};

struct character {
    unsigned int    textureID;  // ID handle of the glyph texture
    vec2            size;       // Size of glyph
    vec2            bearing;    // Offset from baseline to left/top of glyph
    unsigned int    advance;    // Offset to advance to next glyph
};

class Ui_text
{
public:
    FT_Library      ft;
    std::map<char, character>   characters;
    text_t          text_to_draw[5];
    unsigned int	vbo;
    unsigned int	vao;
    unsigned int	ibo;
    unsigned int	texture_id;
    int				shader_id;
public:
    int     init();
    void	add_text_ui(std::string str, Slot slot, float x, float y, float scale);
private:
    int     counter = 0;
private:
    void    set_shader(const char* vPath, const char* fSPath);
    void    vertex_buffer();
};
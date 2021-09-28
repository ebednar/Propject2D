#include "sandbox.h"

void Sandbox::place_ent()
{
    eng->scene.load_resources("res/scene/test.resources");
    eng->scene.load_scene("res/scene/test.scene");
    eng->scene.load_map("res/scene/test.map");
}
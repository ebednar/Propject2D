#include "sandbox.h"

void Sandbox::place_ent(Engine* eng)
{
    eng->scene.load_resources("res/scene/test.resources");
    eng->scene.load_scene("res/scene/test.scene");

    glm::vec3 lightPositions = glm::vec3(0.5f, 0.0f, 1.0f);
    glm::vec3 lightColors = glm::vec3(1.0f, 0.0f, 0.0f);
    eng->add_light_source(lightPositions, lightColors);
}
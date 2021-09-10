#include "sandbox.h"

void Sandbox::place_ent(Engine* eng)
{
    eng->scene.load_scene("res/scene/test.scene");
    Entity  box;
    Model* mod = new Model();
    Model* lightMod = new Model();
    Material* material = new Material;

    mod->load_obj("res/models/sprite.obj", true);
    mod->set_shader("res/shaders/base_vert.glsl", "res/shaders/base_frag.glsl");
    mod->load_texture("res/textures/wall.jpg");

    eng->scene.model_atlas["box"] = mod;

    float color[3] = { 0.0f, 1.0f, 0.0f };
    material->load_material(color, 1.0f, 0.9f, 0.1f);
    mod->set_material(material);

    lightMod->load_obj("res/models/cube.obj", true);
    lightMod->set_shader("res/shaders/light_vertex.glsl", "res/shaders/light_fragment.glsl");
    lightMod->set_material(material);

    eng->scene.model_atlas["light"] = lightMod;

    box.set_model(eng->scene.model_atlas["box"]);
    box.move_to(0.0f, 0.0f, 0.0f);
    box.rotate(0.0f, 0.0f, 0.0f);
    eng->add_entity(&box);

    Entity light;
    glm::vec3 lightPositions = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 lightColors = glm::vec3(300.0f, 300.0f, 300.0f);
    light.set_model(lightMod);
    light.move_to(lightPositions.x, lightPositions.y, lightPositions.z);
    light.scale(0.2f, 0.2f, 0.2f);
    eng->add_entity(&light);
    eng->add_light_source(lightPositions, lightColors);
}
#include "sandbox.h"

void Sandbox::place_ent(Engine* eng)
{
    eng->scene.load_scene("res/scene/test.scene");
    Entity*  box = new Entity();
    Model* mod = new Model();
    Model* lightMod = new Model();
    Material* material = new Material;
    glm::vec3 lightPositions[] = {
        glm::vec3(-5.0f,  0.0f, 0.0f),
        glm::vec3(1.0f,  1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    mod->load_obj("res/models/cube.obj", true);
    mod->set_shader("res/shaders/pbr_vert.glsl", "res/shaders/pbr_frag.glsl");
    mod->load_texture("res/textures/wall.jpg");

    float color[3] = { 0.0f, 1.0f, 0.0f };
    material->load_material(color, 1.0f, 0.9f, 0.1f);
    mod->set_material(material);

    lightMod->load_obj("res/models/cube.obj", true);
    lightMod->set_shader("res/shaders/light_vertex.glsl", "res/shaders/light_fragment.glsl");
    lightMod->set_material(material);

    box->set_model(mod);
    eng->add_entity(box);
    box->move_to(0.0f, 0.0f, 0.0f);
    box->rotate(0.0f, 0.0f, 0.0f);

    Entity* light = new Entity();
    light->set_model(lightMod);
    eng->add_entity(light);
    light->move_to(lightPositions[0].x, lightPositions[0].y, lightPositions[0].z);
    light->scale(0.2f, 0.2f, 0.2f);
    eng->add_light_source(lightPositions[0], lightColors[0]);
}
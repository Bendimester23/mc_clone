//
// Created by bendi on 2022. 06. 27..
//
#pragma once
#ifndef MC_CLONE_GAME_H
#define MC_CLONE_GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include "config.h"
#include "shader/ShaderProgram.h"
#include "render/Camera.h"
#include "world/World.h"

class Game {
    GLFWwindow* window;
    ShaderProgram testShader;
    ShaderProgram wireframeShader;
    Camera camera;

    bool wireframe;
    bool cull_face;

public:
    Game();

    void Destroy();

    void Run();

    void Update(double delta);

    void Render(double delta);

    static Game* GetInstance();

    void HandleKey(int key, __attribute__((unused)) int status, int action, __attribute__((unused)) int mods);

    world::World world;
private:
    static void HandleKeyInput(GLFWwindow* window, int key, int status, int action, int mods);
};


#endif //MC_CLONE_GAME_H

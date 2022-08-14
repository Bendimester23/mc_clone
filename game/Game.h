//
// Created by bendi on 2022.08.13..
//

#ifndef MC_CLONE_2_GAME_H
#define MC_CLONE_2_GAME_H

#include "Window.h"
#include <GLFW/glfw3.h>
#include "./gl/ShaderProgram.h"
#include "./Camera.h"

class Game {
    Window m_Window;
    gl::ShaderProgram shader;
    Camera m_Camera;

    Game();

    ~Game() = default;
public:

    void Run();

    void Quit();

    void Update(double delta);

    static Game* GetInstance();

    static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);
};


#endif //MC_CLONE_2_GAME_H

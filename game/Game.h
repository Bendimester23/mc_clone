//
// Created by bendi on 2022.08.13..
//

#ifndef MC_CLONE_2_GAME_H
#define MC_CLONE_2_GAME_H

#include "Window.h"
#include <GLFW/glfw3.h>
#include "./gl/ShaderProgram.h"
#include "./gl/Texture.h"
#include "./Camera.h"
#include "./renderer/WorldRenderer.h"

class Game {
    Window m_Window;
    gl::ShaderProgram shader;
    Camera m_Camera;
    gl::Texture m_TestTexture;
    renderer::WorldRenderer m_WorldRenderer;
    bool m_Wireframe;

    Game();

    ~Game() = default;
public:

    void Run();

    void Quit();

    void Update(double delta);

    void ToggleWireframeMode();

    static Game* GetInstance();

    static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);
};


#endif //MC_CLONE_2_GAME_H

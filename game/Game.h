//
// Created by bendi on 2022.08.13..
//

#ifndef MC_CLONE_2_GAME_H
#define MC_CLONE_2_GAME_H

#include "Window.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>
#include "./gl/ShaderProgram.h"
#include "./gl/Texture.h"
#include "./Camera.h"
#include "./renderer/WorldRenderer.h"
#include "./renderer/Skybox.h"

class Game {
    Window m_Window;
    gl::ShaderProgram shader;
    Camera m_Camera;
    gl::Texture m_TestTexture;
    renderer::WorldRenderer m_WorldRenderer;
    renderer::Skybox m_Skybox;
    bool m_Wireframe;
    std::thread m_ChunkWorkerThread;
    std::thread m_GenerateWorkerThread;
    bool m_Running;

    Game();

    ~Game() = default;
public:

    void Run();

    void Quit();

    void Update(double delta);

    void ToggleWireframeMode();

    bool IsRunning();

    static Game* GetInstance();

    static void ChunkWorkerStart();

    static void GenerateWorkerStart();

    static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);
};


#endif //MC_CLONE_2_GAME_H

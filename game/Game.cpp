//
// Created by bendi on 2022.08.13..
//

#include "Game.h"

Game *Game::GetInstance() {
    static Game instance;
    return &instance;
}

Game::Game()
: m_Window(1280, 720, "Minecraft Clone 2 | Loading..."), shader("test"), m_Camera(70, 1280.0f/720.0f, 0.01f, 1000.0f, 15, 10, m_Window.GetHandle())
{
    glfwSetKeyCallback(m_Window.GetHandle(), Game::ProcessInput);
    glfwSetInputMode(this->m_Window.GetHandle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Game::Run() {
    if (this->shader.Reload()) {
        this->m_Window.Dispose();
        exit(1);
    }

    double lastTime = glfwGetTime();
    double lastFrameEnd = glfwGetTime();
    double lastDelta = 1.0/60;

    float vertices[] {
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
    };

    unsigned short indicies[] {
        0, 1, 2,
        0, 3, 2,
    };

    GLuint VAO;
    GLuint VBO;
    GLuint indexVBO;

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indexVBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    while (m_Window.IsOpen()) {
        double now = glfwGetTime();
        double delta = now - lastFrameEnd;
        lastFrameEnd = now;

        double fps = 1.0/((delta+lastDelta)/2.0);

        m_Window.SetTitle(fmt::format("Minecraft Clone 2 | FPS: {0:.2f} FrameTime: {1:.4f}", fps, delta));
        if (now - lastTime >= 1.0) {
            spdlog::info("FPS: {0:.2f} FrameTime: {1:.4f}", fps, delta);
            lastTime = now;
        }
        lastDelta = delta;

        Window::PollEvents();

        //Update goes here
        this->Update(delta);

        Window::Clear();

        shader.Bind();
        shader.SetUniformMat4("projectionMatrix", this->m_Camera.GetMatrix());

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        //Rendering code goes here
        glDrawElements(
                GL_TRIANGLES,
                sizeof(indicies)/2,
                GL_UNSIGNED_SHORT,
                nullptr
                );
        glBindVertexArray(0);

        shader.UnBind();
        m_Window.SwapBuffers();
    }

    m_Window.Dispose();
}

void Game::ProcessInput(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;
    if (key == GLFW_KEY_ESCAPE) {
        Game::GetInstance()->Quit();
    }
}

void Game::Update(double delta) {
    this->m_Camera.Update((float)delta);
}

void Game::Quit() {
    m_Window.Close();
}

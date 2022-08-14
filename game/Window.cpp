//
// Created by bendi on 2022.08.13..
//

#include "Window.h"

Window::Window(int width, int height, const std::string &title) {
    if (!glfwInit()) {
        spdlog::error("Could not initialise GLFW!");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwSetErrorCallback([](int, const char *msg) {

        spdlog::error("GLFW error: {}", msg);
    });

    this->m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (m_Window == nullptr) {
        spdlog::error("Failed to create Window!");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(-1);

    if (!gladLoadGL()) {
        spdlog::error("Failed to load OpenGl!");
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        exit(1);
    }

    glClearColor(.3f, .3f, .4f, 1.0f);
    glViewport(0, 0, 1280, 720);
    //TODO turn these back on
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glDepthFunc(GL_LESS);
}

bool Window::IsOpen() {
    return !glfwWindowShouldClose(m_Window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

void Window::Dispose() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    spdlog::info("Stopped");
}

void Window::SetTitle(const std::string &title) {
    glfwSetWindowTitle(m_Window, title.c_str());
}

GLFWwindow *Window::GetHandle() {
    return this->m_Window;
}

void Window::Close() {
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

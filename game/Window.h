//
// Created by bendi on 2022.08.13..
//

#ifndef MC_CLONE_2_WINDOW_H
#define MC_CLONE_2_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include "../config.h"

class Window {
    GLFWwindow* m_Window;

public:
    Window(int width, int height, const std::string &title);

    bool IsOpen();

    static void PollEvents();

    void Clear();

    void SwapBuffers();

    void Dispose();

    void Close();

    void SetTitle(const std::string &title);

    GLFWwindow *GetHandle();
};


#endif //MC_CLONE_2_WINDOW_H

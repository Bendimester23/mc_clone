#include <spdlog/spdlog.h>
//For some unknown reason, the code won't compile, unless glew.h is included before glfw3.h
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "config.h"

void HandleKeyInput(GLFWwindow* window, int key, int status, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    spdlog::info("Starting!");

    if (glfwInit() != GLFW_TRUE) {
        spdlog::error("GLFW init failed!");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Window creation failed!");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, HandleKeyInput);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        spdlog::error("GLEW init failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glClearColor(.3f, .3f, .4f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

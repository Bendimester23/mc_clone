//
// Created by bendi on 2022. 06. 27..
//

#ifndef MC_CLONE_GAME_H
#define MC_CLONE_GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "config.h"
#include "shader/ShaderProgram.h"
#include "render/Camera.h"
#include "render/mesh/MeshBuilder.h"
#include <stb_image.h>

class Game {
    GLFWwindow* window;
    ShaderProgram testShader;
    ShaderProgram wireframeShader;
    ShaderProgram textureShader;
    Camera camera;

    bool wireframe;

    GLuint VBO{}, VAO{}, IVBO{}, texVBO{}, texVAO{}, textureId{};


public:
    Game();

    void Destroy();

    void Run();

    void Update(double delta);

    void Render(double delta);

    static Game* GetInstance();

    void HandleKey(int key, int status, int action, int mods);

private:
    static void HandleKeyInput(GLFWwindow* window, int key, int status, int action, int mods);
};


#endif //MC_CLONE_GAME_H

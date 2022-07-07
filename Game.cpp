//
// Created by bendi on 2022. 06. 27..
//

#include "Game.h"

float ve[] = {
        //pos                  //UV
        0.0f, 1.0f,    0.0f, 0.0f,
        0.0f, 0.0f,    0.0f, 1.0f,
        1.0f, 1.0f,    1.0f, 0.0f,

        1.0f, 1.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 1.0f
};

Game::Game() : testShader("test"), wireframeShader("wireframe"), cull_face(true), world(nullptr) {
    if (glfwInit() != GLFW_TRUE) {
        spdlog::error("GLFW init failed!");
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetErrorCallback([](int, const char * err) {
        spdlog::info("GLFW Error: {}", err);
    });

    this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Window creation failed!");
        glfwTerminate();
        std::exit(1);
    }
    this->camera = Camera(60, ((float) WINDOW_WIDTH) / (float) WINDOW_HEIGHT, 0.001f, 1000.0f, 10, 10, window);
    this->camera.SetPosition(glm::vec3(0.0f, 1.5f, 1.5f));
    this->world.cam = &this->camera;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, Game::HandleKeyInput);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        spdlog::error("GLEW init failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        std::exit(1);
    }

    testShader.Reload();

    glClearColor(.3f, .3f, .4f, 1.0f);
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    this->world.Init();
}

void Game::HandleKeyInput(GLFWwindow *window, int key, int status, int action, int mods) {
    //Only exit in debug
#ifdef EXIT_WITH_ESC
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
#endif
    Game::GetInstance()->HandleKey(key, status, action, mods);
}

void Game::Update(double delta) {
    this->camera.Update((float) delta);
    this->world.Update();
}

void Game::Render(double delta) {
    this->world.Render();
}

void Game::Run() {
    double lastTime = glfwGetTime();
    double lastFrameEnd = glfwGetTime();
    int fps = 0;

    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        fps++;
        double delta = currentTime - lastFrameEnd;
        lastFrameEnd = currentTime;
        if (currentTime - lastTime >= 1.0) {
            spdlog::info("FPS: {} FrameTime: {}", fps, delta);
            fps = 0;
            lastTime = currentTime;
        }

        glfwPollEvents();
        this->Update(delta);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->Render(delta);

        glfwSwapBuffers(window);
    }
}

void Game::Destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

Game *Game::GetInstance() {
    static Game GAME;
    return &GAME;
}

void Game::HandleKey(int key, __attribute__((unused)) int status, int action, __attribute__((unused)) int mods) {
    if (action != GLFW_PRESS) return;
    switch (key) {
        case GLFW_KEY_F1:
            this->wireframe = !this->wireframe;
            if (this->wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;

        case GLFW_KEY_F2:
            this->cull_face = !this->cull_face;
            if (this->cull_face) {
                glEnable(GL_CULL_FACE);
            } else {
                glDisable(GL_CULL_FACE);
            }
            break;

        case GLFW_KEY_F3:
            this->camera.ToggleCursorState();
            break;

        case GLFW_KEY_F5:
            testShader.Reload();
            wireframeShader.Reload();
            break;

        default:
            break;
    }
}

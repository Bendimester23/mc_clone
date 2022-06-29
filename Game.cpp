//
// Created by bendi on 2022. 06. 27..
//

#include "Game.h"


float vertices[] = {
        -1.0f, -1.0f, 1.0f, //bal alul - e
        1.0f, 1.0f, 1.0f,   //bal felül - e
        1.0f, -1.0f, 1.0f,  //jobb alul - e
        -1.0f, 1.0f, 1.0f, //jobb felül - e

        -1.0f, -1.0f, -1.0f, //bal alul -  h
        1.0f, 1.0f, -1.0f,   //bal felül - h
        1.0f, -1.0f, -1.0f,  //jobb alul - h
        -1.0f, 1.0f, -1.0f, //jobb felül - h
};

unsigned short indices[] = {
        //front
        0, 2, 1,
        0, 1, 3,

        //back
        4, 5, 6,
        4, 7, 5,

        //top
        1, 5, 3,
        7, 3, 5,

        //bottom
        0, 4, 2,
        6, 2, 4,

        //left
        0, 5, 1,
        0, 4, 5,

        //right
        2, 7, 3,
        2, 6, 7,
};

Game::Game() : testShader("test") {
    if (glfwInit() != GLFW_TRUE) {
        spdlog::error("GLFW init failed!");
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Window creation failed!");
        glfwTerminate();
        std::exit(1);
    }
    this->camera = Camera(60, ((float)WINDOW_WIDTH)/(float)WINDOW_HEIGHT, 0.001f, 1000.0f, 20, 10, window);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(-1);

    glfwSetKeyCallback(window, Game::HandleKeyInput);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::HandleKeyInput(GLFWwindow *window, int key, int status, int action, int mods) {
    //Only exit in debug
#ifdef EXIT_WITH_ESC
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
#endif
}

void Game::Update(double delta) {
    this->camera.Update((float)delta);
}

float speed = 10;

void Game::Render(double delta) {
    testShader.Bind();
    testShader.SetUniformMat4("projectionMat", this->camera.GetMatrix());

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IVBO);
    glDrawElements(
            GL_TRIANGLES,
            sizeof(indices)/sizeof(unsigned short),
            GL_UNSIGNED_SHORT,
            (void*)nullptr
            );
    glBindVertexArray(0);
    testShader.UnBind();
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

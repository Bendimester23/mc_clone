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

MeshBuilder mb;

Game::Game() : testShader("test"), wireframeShader("wireframe"), textureShader("texture"), cull_face(true) {
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
    this->camera = Camera(60, ((float) WINDOW_WIDTH) / (float) WINDOW_HEIGHT, 0.001f, 1000.0f, 20, 10, window);

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
    wireframeShader.Reload();
    textureShader.Reload();

    glClearColor(.3f, .3f, .4f, 1.0f);
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    for (int x = 0; x < 32; ++x) {
        for (int z = 0; z < 32; ++z) {
            mb.AddQuad(
                    VertexCoord(x,   0,   z  ),
                    VertexCoord(x+1, 0,   z  ),
                    VertexCoord(x+1, 0,   z+1),
                    VertexCoord(x,   0,   z+1),
                    false
                    );
        }
    }

    this->testMesh.From(mb);

    int width, height, channels;
    unsigned char *img = stbi_load("../assets/textures/test.png", &width, &height, &channels, 0);
    if(img == nullptr) {
        printf("Error in loading the image\n");
        exit(1);
    }
    spdlog::info("Loaded image with a width of {}px, a height of {}px and {} channels", width, height, channels);

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glGenVertexArrays(1, &texVAO);
    glGenBuffers(1, &texVBO);
    glBindVertexArray(texVAO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, (long)sizeof(ve), ve, GL_STATIC_DRAW);

    //Pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    //Uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2*sizeof(float)));
    glEnableVertexAttribArray(1);

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
    Game::GetInstance()->HandleKey(key, status, action, mods);
}

void Game::Update(double delta) {
    this->camera.Update((float) delta);
}

void Game::Render(double delta) {
    if (this->wireframe) {
        wireframeShader.Bind();
        wireframeShader.SetUniformMat4("projectionMat", this->camera.GetMatrix());
    } else {
        testShader.Bind();
        testShader.SetUniformMat4("projectionMat", this->camera.GetMatrix());
    }
    this->testMesh.Bind();
    glDrawElements(
            GL_TRIANGLES,
            mb.GetIndicesCount(),
            GL_UNSIGNED_SHORT,
            (void *) nullptr
    );
    this->testMesh.UnBind();
    if (this->wireframe) {
        wireframeShader.UnBind();
    } else {
        testShader.UnBind();
    }

    textureShader.Bind();

    glBindVertexArray(texVAO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(ve)/sizeof(float));
    glBindVertexArray(0);

    textureShader.UnBind();
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

void Game::HandleKey(int key, int status, int action, int mods) {
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

        default:
            break;
    }
}

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#include <spdlog/spdlog.h>
#include "Game.h"
//For some unknown reason, the code won't compile, unless glew.h is included before glfw3.h

int main() {
    spdlog::info("Starting!");

    Game::GetInstance()->Run();
    return 0;
}

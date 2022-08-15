//stbi preprocesor stuff that is very important
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#include <spdlog/spdlog.h>
#include "game/Game.h"

int main() {
    spdlog::info("Starting...");
    Game::GetInstance()->Run();
    return 0;
}

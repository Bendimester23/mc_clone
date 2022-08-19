//stbi preprocesor stuff that is very important
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION


#ifdef __WIN32
extern "C" {
// Enable dedicated graphics
__declspec(dllexport) bool NvOptimusEnablement = true;
__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
}
#endif // __WIN32

#include <spdlog/spdlog.h>
#include "game/Game.h"

int main() {
    spdlog::info("Starting...");
    Game::GetInstance()->Run();
    return 0;
}

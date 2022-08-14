#include <spdlog/spdlog.h>
#include "game/Game.h"

int main() {
    spdlog::info("Starting...");
    Game::GetInstance()->Run();
    return 0;
}

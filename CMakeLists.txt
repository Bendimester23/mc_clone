cmake_minimum_required(VERSION 3.23)
project(mc_clone_2)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdparty/glad/)
add_subdirectory(thirdparty/glfw/)
add_subdirectory(thirdparty/glm/)
add_subdirectory(thirdparty/spdlog/)

add_executable(mc_clone_2 main.cpp game/Window.cpp game/Window.h game/Game.cpp game/Game.h game/gl/ShaderProgram.cpp game/gl/ShaderProgram.h config.h utils/files.h utils/files.cpp game/Camera.cpp game/Camera.h)

target_include_directories(mc_clone_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include)
target_include_directories(mc_clone_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog/include)
target_include_directories(mc_clone_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm)
target_include_directories(mc_clone_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb)

target_link_libraries(mc_clone_2 PRIVATE glad glfw glm::glm spdlog)

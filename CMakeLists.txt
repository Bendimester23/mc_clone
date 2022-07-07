cmake_minimum_required(VERSION 3.21)
project(mc_clone)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory( thirdparty/glfw )
add_subdirectory( thirdparty/glew )
add_subdirectory( thirdparty/spdlog )
add_subdirectory( thirdparty/glm )

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()


add_executable(mc_clone
        main.cpp
        config.h
        Game.cpp
        Game.h
        shader/ShaderProgram.cpp
        shader/ShaderProgram.h
        utils/files.h
        utils/files.cpp
        render/Camera.cpp render/Camera.h render/mesh/MeshBuilder.cpp render/mesh/MeshBuilder.h render/mesh/Mesh.cpp render/mesh/Mesh.h world/Chunk.cpp world/Chunk.h world/World.cpp world/World.h render/mesh/ChunkMesh.cpp render/mesh/ChunkMesh.h render/mesh/ChunkMeshBuilder.cpp render/mesh/ChunkMeshBuilder.h render/texture/Texture.cpp render/texture/Texture.h world/block/Block.cpp world/block/Block.h world/types.h world/types.cpp)

target_include_directories(mc_clone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include)
target_include_directories(mc_clone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glew/include)
target_include_directories(mc_clone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog/include)
target_include_directories(mc_clone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm)
target_include_directories(mc_clone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb)
target_include_directories(mc_clone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/perlin-noise)
target_link_libraries(mc_clone glfw libglew_static spdlog glm::glm)

file(COPY ./assets DESTINATION ./)

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT mc-clone )
    endif()
endif()

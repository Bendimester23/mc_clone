//
// Created by bendi on 2022. 07. 07..
//

#pragma once
#ifndef MC_CLONE_TYPES_H
#define MC_CLONE_TYPES_H

#include <string>
#include "glm/vec3.hpp"

struct ChunkCoordinate {
    ChunkCoordinate(int pX, int pY, int pZ);

    int x, y, z;

public:
    bool operator ==(ChunkCoordinate& other) const;
};

struct BlockCoordinate {
    BlockCoordinate(char pX, char pY, char pZ);

    char x, y, z;
public:
    glm::vec3 operator +(ChunkCoordinate& other);
};

struct SimpleUV {
    char u;
    char v;

    SimpleUV(char pU, char pV);
};


#endif //MC_CLONE_TYPES_H

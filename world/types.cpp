//
// Created by bendi on 2022. 07. 07..
//
#include "./types.h"

SimpleUV::SimpleUV(char pU, char pV) {
    this->u = pU;
    this->v = pV;
}

ChunkCoordinate::ChunkCoordinate(int pX, int pY, int pZ) {
    this->x = pX;
    this->y = pY;
    this->z = pZ;
}

bool ChunkCoordinate::operator==(ChunkCoordinate &other) const {
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

BlockCoordinate::BlockCoordinate(char pX, char pY, char pZ) {
    this->x = pX;
    this->y = pY;
    this->z = pZ;
}

glm::vec3 BlockCoordinate::operator+(ChunkCoordinate &other) {
    return {this->x+other.x*16, this->y+other.y*16, this->z+other.z*16};
}

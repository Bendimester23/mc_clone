//
// Created by bendi on 2022. 06. 27..
//
#include "files.h"

namespace utils {
    std::string ReadFileToString(const char* path) {
        std::ifstream file;
        file.open(path, std::ios::in);
        if (file.fail()) {
            return "";
        }
        std::stringstream content;
        content << file.rdbuf();
        return content.str();
    }
}

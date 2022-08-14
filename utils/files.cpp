//
// Created by bendi on 2022.08.13..
//
#include "./files.h"

namespace utils {
    std::string ReadFile(const std::string& path) {
        std::ifstream file;
        file.open(path.c_str(), std::ios::in);
        if (file.fail()) {
            return "";
        }
        std::stringstream content;
        content << file.rdbuf();
        return content.str();
    }
}

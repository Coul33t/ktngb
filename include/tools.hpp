#pragma once

#include <cstdio>
#include "unistd.h"

#include <string>

#include "mytypes.hpp"

namespace Tools {
    inline uint hexToDec(std::string hexval) {
        return std::stoi(hexval, nullptr, 16);
    }

    inline void getCWD() {
        char* cwd;
        cwd = (char*) malloc( FILENAME_MAX * sizeof(char) );
        std::cout << getcwd(cwd,FILENAME_MAX) << std::endl;
    }

}
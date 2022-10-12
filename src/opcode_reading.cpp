#include "../include/opcode_reading.hpp"

namespace OpcodeReading {
    json readJsonFile(std::string filename) {
        std::ifstream i(filename);
        json j;
        i >> j;
        return j;
    }

}
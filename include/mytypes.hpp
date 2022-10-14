#pragma once

#include "fmt/core.h"

typedef unsigned int uint;
typedef uint8_t byte;
typedef unsigned char byte_as_char;

struct ByteArray {
    byte_as_char* array;
    size_t size;

    ByteArray() = default;

    // Copy ctr
    ByteArray(const ByteArray& other) {
        this->array = new byte_as_char[other.size];
        for (size_t i = 0; i < other.size; i++) {
            this->array[i] = other.array[i];
        }

        this->size = other.size;
    }

    ByteArray(byte_as_char* array, size_t size):
            array(array), size(size) {}

    std::string getHexValue() {
        std::string str;

        str += "0x";

        for (size_t i = 0; i < size; i++) {
            str += fmt::format("{:x}", static_cast<int>(array[i]));
        }

        return str;
    }
};

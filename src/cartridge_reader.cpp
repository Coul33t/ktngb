//
// Created by couland-q on 12/10/22.
//

#include "../include/cartridge_reader.hpp"

void CartReader::readData(const std::string& filename) {
    // Reading is from 0x100 to 0x14F
    std::ifstream cart_file;
    cart_file.open(filename, std::ios::in | std::ios::binary);

    // entrypoint (0x100 - 0x103)
    readMemory(cart_file, 0x100, 0x103, "entrypoint");
}

void CartReader::readMemory(std::ifstream& cart_file, int beg, int end, const std::string& name) {
    char* memblock = new char[end - beg];
    cart_file.read(memblock, beg);

    this->meta.data.insert(std::make_pair(name, std::string(memblock)));
    free(memblock);
}

void CartReader::bigToLittleEndian(char* array, size_t array_size) {
    // Swap each pair of bytes
    for (size_t i = 0; i < array_size; i += BYTE_SIZE * 4) {
        // swap
    }
}
//
// Created by couland-q on 12/10/22.
//

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "mytypes.hpp"
#include "constants.hpp"

struct CartMetaData {
    std::map<std::string, std::string> data;

    void displayMetaData() {
        for (auto& item: data) {
            std::cout << item.first << ": " << item.second << std::endl;
        }
    }
};

class CartReader {
public:
    void readData(const std::string& filename);
    void readMemory(std::ifstream& cart_file, int beg, int end, const std::string& name);
    void bigToLittleEndian(char* array, size_t array_size);

    CartMetaData meta;
};

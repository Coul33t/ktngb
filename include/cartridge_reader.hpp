//
// Created by couland-q on 12/10/22.
//

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <fstream>


#include "instruction_list.hpp"
#include "mytypes.hpp"
#include "constants.hpp"

struct CartMetaData {
    std::map<std::string, ByteArray*> data;
    std::vector<std::string> insertion_order;

    ~CartMetaData() {
        for (auto& elem: data) {
            delete elem.second;
        }
    }

    void displayMetaData() {
        for (auto& key: insertion_order) {
            std::cout << key << ": " << std::hex;
            for (size_t i = 0; i < data[key]->size; i++) {
                std::cout << static_cast<int>(data[key]->array[i]) << " ";
            }
            std::cout << std::dec << std::endl;
        }
    }
};

class CartReader {
public:
    void init(const std::string& filename);
    void stop();

    char* readMemory(uint address, size_t nb_bytes);
    byte_as_char* convertToByteAsChar(char* memblock, size_t nb_bytes);
    byte_as_char* readAndConvert(uint address, size_t nb_bytes);
    byte_as_char readAndConvertSingleByte(uint address);

    uint decodeInstr(uint address, InstructionsList& instr_lst);

    void readMetaData();
    void readMetaDataMemory(uint beg, uint end, const std::string& name);

    bool checkNintendoLogo(bool display=false);

    CartMetaData meta;
    std::ifstream cart_file;
};

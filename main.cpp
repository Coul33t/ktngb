// https://www.inspiredpython.com/course/game-boy-emulator

// <addr> <opcode> <mnemonic> [<operand> ...] [; commentary ]

#include <iostream>

#include "include/instruction_list.hpp"
#include "include/cartridge_reader.hpp"

int main() {
    std::cout << "Hello world!" << std::endl;
    InstructionsList instr_lst;
    instr_lst.readJSONtoStruct("../opcodes.json");
    CartReader cart_reader;
    cart_reader.readData("../res/Boxes.gb");
    cart_reader.meta.displayMetaData();
    return 0;
}
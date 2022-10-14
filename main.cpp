// https://www.inspiredpython.com/course/game-boy-emulator

// <addr> <opcode> <mnemonic> [<operand> ...] [; commentary ]

// TODO: endian is wrong (0x152 operand value should be 0xfffe, is 0xfeff for now)

#include <iostream>

#include "include/instruction_list.hpp"
#include "include/cartridge_reader.hpp"

int main() {
    std::cout << "Hello world!" << std::endl;

    // Loading opcodes
    InstructionsList instr_lst;
    instr_lst.readJSONtoStruct("../opcodes.json");

    // Loading the cartridge (= opening the stream)
    CartReader cart_reader;
    cart_reader.init("../res/snake.gb");
    cart_reader.readMetaData();
    cart_reader.meta.displayMetaData();
    cart_reader.checkNintendoLogo(true);

    uint address = 0x150;
    while(address < 0x180) {
        address = cart_reader.decodeInstr(address, instr_lst);
    }

    cart_reader.stop();

    return 0;
}
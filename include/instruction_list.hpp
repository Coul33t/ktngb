#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <set>

#include "../extlib/json/json.hpp"

#include "global_includes.hpp"
#include "tools.hpp"

struct Operand {
    std::string name;
    uint bytes = 0;
    bool immediate = false;

    Operand() = default;
};

struct Instruction {
    uint16_t opcode = 0;
    std::string mnemonic;
    uint bytes = 0;
    std::vector<uint> cycles;
    bool immediate = false;
    std::map<std::string, std::string> flags;
    std::vector<Operand> operands;

    Instruction() = default;

    friend std::ostream& operator<<(std::ostream& os, const Instruction& m) {
        std::string str;
        str += "opcode   : " + std::to_string(m.opcode) + "\n";
        str += "mnemonic : " + m.mnemonic + "\n";
        str += "bytes    : " + std::to_string(m.bytes) + "\n";
        str += "cycles   : \n";

        for (auto& e: m.cycles) {
            str += "           " + std::to_string(e) + "\n";
        }

        str += "operands : \n";
        for (auto& op: m.operands) {
            str += "    name      : " + op.name + "\n";
            str += "    bytes     : " + std::to_string(op.bytes) + "\n";
            str += "    immediate : " + std::to_string(op.immediate) + "\n\n";
        }

        str += "immediate: " + std::to_string(m.immediate) + "\n";

        str += "flags    : \n";
        for (auto& f: m.flags) {
            str += "           " + f.first + ": " + f.second + "\n";
        }

        return os << str;
    }
};

class InstructionsList {
public:
    /**
     * Reads a JSON containing all opcodes and put them into Instruction structures (with the help of the
     * getAllInstructions() function, see below).
     * @param filename the path to the JSON file containing all the opcodes
     */
    void readJSONtoStruct(const std::string& filename);

    /**
     * Returns a vector containing all the opcode of a specified type (either "unprefixed" or "cbprefixed") read from
     * a JSON object.
     * @param json_object The JSON object containing all the opcodes
     * @param type Type of the opcodes (either "unprefixed" or "cbprefixed")
     * @return
     */
    std::vector<Instruction> getAllInstructions(nlohmann::json json_object, const std::string& type);

    Instruction getOpcode(uint opcode_idx, const std::string& type);
    /**
     * Helper function to display all the Opcode/Operands keys, and all flags.
     * @param json_object The JSON object with load opcode data
     */
    void getAllOperandsKeys(nlohmann::json json_object);



    std::vector<Instruction> unprefixed;
    std::vector<Instruction> cbprefixed;
};


#include "../include/instruction_list.hpp"

/*
    uint16_t opcode;
    std::string mnemonic;
    uint bytes;
    std::vector<uint> cycles;
    bool immediate;
    std::map<std::string, std::string> flags;
    std::vector<Operand> operands;
*/

void InstructionsList::readJSONtoStruct(const std::string& filename) {
    // Don't polute the whole file with this
    using json = nlohmann::json;
    
    std::ifstream i(filename);
    //std::cout << "cout:" << std::endl;
    //std::cout << i.rdbuf();

    json json_object;
    i >> json_object;

    //getAllOperandsKeys(json_object);

    unprefixed = getAllInstructions(json_object, InstrType::Unprefixed);
    cbprefixed = getAllInstructions(json_object, InstrType::CBPrefixed);

}

std::vector<Instruction> InstructionsList::getAllInstructions(nlohmann::json json_object, const InstrType& instr_type) {
    std::vector<Instruction> instruct_vect;

    std::string type = getEnumStr(instr_type);

    for (auto& it_inst : json_object[type].items()) {
        Instruction inst;

        inst.opcode = Tools::hexToDec(it_inst.key());
        inst.mnemonic = it_inst.value()["mnemonic"];
        inst.bytes = it_inst.value()["bytes"];

        for (auto& val: it_inst.value()["cycles"]) {
            inst.cycles.push_back(val);
        }

        inst.immediate = it_inst.value()["immediate"];
        inst.flags = it_inst.value()["flags"];

        for (auto& it_ope : it_inst.value()["operands"].items()) {
            Operand ope;
            ope.name = it_ope.value()["name"];

            if (it_ope.value().contains("bytes"))
                ope.bytes = it_ope.value()["bytes"];

            if (it_ope.value().contains("immediate"))
                ope.immediate = it_ope.value()["immediate"];

            inst.operands.push_back(ope);
        }

        instruct_vect.push_back(inst);
    }

    return instruct_vect;
}

Instruction InstructionsList::getOpcode(uint opcode_idx, const InstrType& instr_type) {

    std::string type = getEnumStr(instr_type);

    if (type != "unprefixed" && type != "cbprefixed") {
        std::cout << "ERROR: either type is wrong "
                  << "(got " << type << ", expected either \"unprefixed\" or \"cbprefixed\") "
                  << std::endl;
        return {};
    }

    if ((type == "unprefixed" && opcode_idx > unprefixed.size() - 1) ||
        (type == "cbprefixed" && opcode_idx > cbprefixed.size() - 1)) {
        std::cout << "opcode_idx is too high "
                  << "(got " << opcode_idx << ", last index is " << unprefixed.size() - 1
                  << " for unprefixed or "<< cbprefixed.size() - 1 <<" for cbprefixed)"
                  << std::endl;
        return {};
    }

    if (type == "unprefixed" && opcode_idx < unprefixed.size() - 1)
        return unprefixed[opcode_idx];

    else if (type == "cbprefixed" && opcode_idx < cbprefixed.size() - 1)
        return cbprefixed[opcode_idx];
}

void InstructionsList::getAllOperandsKeys(nlohmann::json json_object) {
    std::set<std::string> all_opcode_keys;
    std::set<std::string> all_operands_keys;
    std::set<std::string> all_flags;

    // For each opcode
    for (auto& it_inst : json_object["unprefixed"].items()) {
        // For each key in an opcode
        for (auto& it_second: json_object["unprefixed"][it_inst.key()].items()) {
            if (it_second.key() == "operands") {
                // For each operand in an opcode
                for (auto& it_ope : it_inst.value()["operands"].items()) {
                    // For each key in the operand
                    for (auto& it_ope_items: it_ope.value().items()) {
                        all_operands_keys.insert(it_ope_items.key());
                    }
                }
            }

            if (it_second.key() == "flags") {
                for (auto& it_ope : it_inst.value()["flags"].items()) {
                    all_flags.insert(it_ope.key());
                }
            }

            all_opcode_keys.insert(it_second.key());
        }
    }

    std::cout << "All opcode keys:" << std::endl;
    for (auto& elem: all_opcode_keys) {
        std::cout << "  - " << elem << std::endl;
    }

    std::cout << "All operands keys:" << std::endl;
    for (auto& elem: all_operands_keys) {
        std::cout << "  - " << elem << std::endl;
    }

    std::cout << "All flags:" << std::endl;
    for (auto& elem: all_flags) {
        std::cout << "  - " << elem << std::endl;
    }
}

std::string InstructionsList::getEnumStr(const InstrType& type) {
    if (type == InstrType::Unprefixed) {
        return std::string{"unprefixed"};
    }

    else if (type == InstrType::CBPrefixed) {
        return std::string{"cbprefixed"};
    }
}
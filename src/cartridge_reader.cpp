//
// Created by couland-q on 12/10/22.
//

#include "../include/cartridge_reader.hpp"

void CartReader::init(const std::string& filename) {
    cart_file.open(filename, std::ios::in | std::ios::binary);
}

void CartReader::stop() {
    cart_file.close();
}
void CartReader::readMetaData() {
    // Reading metadata is from 0x100 to 0x14F
    // entrypoint (0x100 - 0x103)
    readMetaDataMemory(0x100, 0x103, "entrypoint");

    // Nintendo logo (0x104 - 0x133)
    readMetaDataMemory(0x104, 0x133, "Nintendo logo");

    // Cartridge title (0x134 - 0x142) (0x143 is shared with the cgb flag)
    readMetaDataMemory(0x134, 0x142, "cartridge title");

    // cgb flag (0x143)
    readMetaDataMemory(0x143, 0x143, "cgb flag");

    // new licensee code (0x144 - 0x145)
    readMetaDataMemory(0x144, 0x145, "new licensee code");

    // sgb flag (0x146)
    readMetaDataMemory(0x146, 0x146, "sgb flag");

    // cartridge type (0x147)
    readMetaDataMemory(0x147, 0x147, "cartridge type");

    // rom size (0x148)
    readMetaDataMemory(0x148, 0x148, "rom size");

    // ram size (0x149)
    readMetaDataMemory(0x144, 0x145, "ram size");

    // destination code (0x14A)
    readMetaDataMemory(0x14A, 0x14A, "destination code");

    // old licensee code (0x14B)
    readMetaDataMemory(0x14B, 0x14B, "old licensee code");

    // mask rom version (0x14C)
    readMetaDataMemory(0x14C, 0x14C, "mask rom version");

    // header checksum (0x144 - 0x145)
    readMetaDataMemory(0x14D, 0x14D, "header checksum");

    // global checksum (0x14E - 0x14F)
    readMetaDataMemory(0x14E, 0x14F, "global checksum");
}

char* CartReader::readMemory(uint address, size_t nb_bytes) {
    cart_file.seekg(address);
    auto memblock = new char[nb_bytes];
    cart_file.read(memblock, static_cast<std::streamsize>(nb_bytes));
    return memblock;
}

byte_as_char* CartReader::convertToByteAsChar(char* memblock, size_t nb_bytes) {
    auto converted_memblock = new byte_as_char[nb_bytes];

    for (size_t i = 0; i < nb_bytes; i++) {
        converted_memblock[i] = static_cast<byte_as_char>(memblock[i]);
    }

    return converted_memblock;
}

byte_as_char* CartReader::readAndConvert(uint address, size_t nb_bytes) {
    char* memblock = readMemory(address, nb_bytes);
    byte_as_char* converted_memblock = convertToByteAsChar(memblock, nb_bytes);
    delete[] memblock;
    return converted_memblock;
}

byte_as_char CartReader::readAndConvertSingleByte(uint address) {
    char* memblock = readMemory(address, 1);
    byte_as_char converted_memblock = convertToByteAsChar(memblock, 1)[0];
    delete[] memblock;
    return converted_memblock;
}

uint CartReader::decodeInstr(uint address, InstructionsList& instr_lst) {
    byte_as_char opcode = readAndConvertSingleByte(address);
    std::cout << std::hex << address << std::dec << " ";
    address += 1;

    Instruction new_instr;
    Instruction opcode_instr;

    // If opcode is CB, it means that the actual opcode is the next one
    // in the cbprefixed list
    if (opcode == 0xCB) {
        opcode = readAndConvertSingleByte(address);
        opcode_instr = instr_lst.getOpcode(opcode, InstrType::CBPrefixed);
    }

    // else, the unprefixed version (so no need to shift the address)
    else {
        opcode_instr = instr_lst.getOpcode(opcode, InstrType::Unprefixed);
    }

    new_instr = opcode_instr;
    std::cout << new_instr.mnemonic <<  " (" << std::hex<< static_cast<int>(opcode) << std::dec <<  ")\t\t";

    for (auto& op: new_instr.operands) {
        if (op.bytes > 0) {
            op.value = ByteArray(readAndConvert(address, op.bytes), op.bytes);
            address += op.bytes;
            std::cout << op.value.getHexValue();
        }

        else {
            std::cout << op.name;
        }

        std::cout << ", ";
    }

    std::cout << std::endl;

    return address;
}

void CartReader::readMetaDataMemory(uint beg, uint end, const std::string& name) {
    size_t nb_bytes = end - beg + 1;
    auto converted_memblock = readAndConvert(beg, nb_bytes);

    this->meta.data.insert(std::make_pair(name, new ByteArray(converted_memblock, nb_bytes)));
    this->meta.insertion_order.push_back(name);
}

bool CartReader::checkNintendoLogo(bool display) {
    if (meta.data["Nintendo logo"]->size != NINTENDO_LOGO_SIZE) {
        if (display) {
            std::cout << "ERROR: Nintendo logo size is wrong (expected size is " << NINTENDO_LOGO_SIZE
                      << ", cartridge size is " << meta.data["Nintendo logo"]->size << ")." << std::endl;
            return false;
        }
    }

    for (size_t i = 0; i < meta.data["Nintendo logo"]->size; i++) {
        if (meta.data["Nintendo logo"]->array[i] != NINTENDO_LOGO_DUMP[i]) {
            if (display) {
                std::cout << "ERROR: expected " << std::hex << NINTENDO_LOGO_DUMP[i] << ", got "
                          << meta.data["Nintendo logo"]->array[i] << " instead." << std::endl;
            }

            return false;
        }
    }

    if (display) {
        std::cout << "Cartridge's Nintendo logo is correct." << std::endl;
    }

    return true;
}
#pragma once

#include <fstream>

#include "../extlib/json/json.hpp"

using json = nlohmann::json;

namespace OpcodeReading {
    json readJsonFile(std::string filename);
}
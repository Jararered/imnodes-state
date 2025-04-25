#pragma once

#include "NodeIO.hpp"

#include <cstdint>
#include <string>
#include <vector>

struct Node
{
    uint32_t ID;
    std::string Name;
    std::vector<NodeIO> Inputs;
    std::vector<NodeIO> Outputs;
    std::vector<uint32_t> LinkIDs;
};
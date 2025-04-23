#pragma once

#include <cstdint>
#include <string>

struct NodeIO
{
    uint32_t NodeID = 0;
    uint32_t ID = 0;
    std::string Name = "";

    NodeIO(uint32_t nodeId, uint32_t id, std::string name) : NodeID(nodeId), ID(id), Name(name) {}
};
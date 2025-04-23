#pragma once

#include <cstdint>
#include <string>

struct NodeIO
{
    uint32_t m_NodeId = 0;
    uint32_t m_Id = 0;
    std::string m_Name = "";

    NodeIO(uint32_t nodeId, uint32_t id, std::string name) : m_NodeId(nodeId), m_Id(id), m_Name(name) {}
};
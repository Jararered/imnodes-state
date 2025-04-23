#pragma once

#include "NodeIO.hpp"

#include <cstdint>
#include <string>
#include <vector>

struct Node
{
    uint32_t m_Id;
    std::string m_Name;
    std::vector<NodeIO> m_Inputs;
    std::vector<NodeIO> m_Outputs;

    void Render();
};
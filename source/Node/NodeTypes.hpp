#pragma once

#include <cstdint>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using NodeIDSet = std::set<uint32_t>;
using PinIDSet = std::set<uint32_t>;
using LinkIDSet = std::set<uint32_t>;
using NodeIDQueue = std::queue<uint32_t>;
using PinIDQueue = std::queue<uint32_t>;
using LinkIDQueue = std::queue<uint32_t>;

enum class PinType
{
    Input = 0,
    Output = 1
};

struct PinData
{
    std::string Name;
    uint32_t NodeID;
    PinType Type;
    LinkIDSet Links;
};

struct NodeData
{
    std::string Name;
    std::pair<float, float> Position;
    PinIDSet InputIDs;
    PinIDSet OutputIDs;
};

struct LinkData
{
    uint32_t Pin1ID;
    uint32_t Pin2ID;
};

using PinDataMap = std::unordered_map<uint32_t, PinData>;
using NodeDataMap = std::unordered_map<uint32_t, NodeData>;
using LinkDataMap = std::unordered_map<uint32_t, LinkData>;
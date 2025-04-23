#pragma once

#include "Node.hpp"

#include <unordered_map>
#include <unordered_set>

class NodeManager
{
private:
    // Renderables
    std::unordered_map<uint32_t, Node> m_Nodes;
    std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> m_Links;

    // IDs
    std::unordered_set<uint32_t> m_UsedNodeIds;
    std::unordered_set<uint32_t> m_UsedIOIds;
    std::unordered_set<uint32_t> m_UsedLinkIds;

    // Next IDs
    uint32_t m_NextNodeId = 0;
    uint32_t m_NextIOId = 0;
    uint32_t m_NextLinkId = 0;

public:
    NodeManager();

    uint32_t CreateNode(const std::string &name);
    uint32_t CreateLink(uint32_t inputId, uint32_t outputId);

    void RemoveNode(uint32_t nodeId);
    void RemoveLink(uint32_t linkId);

    uint32_t AddInput(uint32_t nodeId, std::string name);
    uint32_t AddOutput(uint32_t nodeId, std::string name);

    const void Render() const;
    const void RenderDetails() const;
};
#pragma once

#include "Node.hpp"

#include <unordered_map>
#include <unordered_set>
#include <memory>

class NodeManager
{
private:
    // Renderables
    // <Node ID, Node>
    using NodePtr = std::shared_ptr<Node>;
    using NodeMap = std::unordered_map<uint32_t, NodePtr>;
    NodeMap m_NodeMap;

    // <Link ID, <Input IO ID, Output IO ID>>
    using LinkMap = std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>>;
    LinkMap m_LinkMap;

    // Used IDs
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

    uint32_t AddInput(uint32_t nodeId, const std::string &name);
    uint32_t AddOutput(uint32_t nodeId, const std::string &name);

    NodeMap &GetNodeMap() { return m_NodeMap; }
    LinkMap &GetLinkMap() { return m_LinkMap; }
    std::unordered_set<uint32_t> &GetUsedNodeIds() { return m_UsedNodeIds; }
    std::unordered_set<uint32_t> &GetUsedIOIds() { return m_UsedIOIds; }
    std::unordered_set<uint32_t> &GetUsedLinkIds() { return m_UsedLinkIds; }
};
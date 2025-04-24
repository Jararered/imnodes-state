#pragma once

#include "Node.hpp"

#include <unordered_map>
#include <set>
#include <memory>

class NodeManager
{
private:
    // Unique Node IDs
    using NodePtr = std::shared_ptr<Node>;
    using NodeMap = std::unordered_map<uint32_t, NodePtr>;
    NodeMap m_Nodes;

    // Unique IO IDs
    using IOSet = std::set<uint32_t>;
    IOSet m_IOs;

    // Unique Link IDs
    using Link = std::pair<uint32_t, uint32_t>;
    using LinkMap = std::unordered_map<uint32_t, Link>;
    LinkMap m_Links;

    // Next IDs
    uint32_t m_NextNodeId = 0;
    uint32_t m_NextIOId = 0;
    uint32_t m_NextLinkId = 0;

public:
    NodeManager();

    // Node Management
    uint32_t CreateNode();
    uint32_t CreateNode(const std::string &name);
    void RemoveNode(uint32_t nodeId);

    // Link Management
    uint32_t CreateLink(uint32_t inputId, uint32_t outputId);
    void RemoveLink(uint32_t linkId);

    // IO Management
    uint32_t AddInput(uint32_t nodeId);
    uint32_t AddInput(uint32_t nodeId, const std::string &name);
    uint32_t AddOutput(uint32_t nodeId);
    uint32_t AddOutput(uint32_t nodeId, const std::string &name);
    void RemoveInput(uint32_t nodeId, uint32_t inputId);
    void RemoveOutput(uint32_t nodeId, uint32_t outputId);
    void RenameInput(uint32_t nodeId, uint32_t inputId, const std::string &name);
    void RenameOutput(uint32_t nodeId, uint32_t outputId, const std::string &name);

    NodeMap &GetNodeMap() { return m_Nodes; }
    IOSet &GetIOs() { return m_IOs; }
    LinkMap &GetLinkMap() { return m_Links; }

    bool NodeExists(uint32_t nodeId);
    bool IOExists(uint32_t ioId);
    bool LinkExists(uint32_t linkId);
};
#pragma once

#include "Node.hpp"

#include <unordered_map>
#include <set>
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
    std::set<uint32_t> m_RegisteredNodes;
    std::set<uint32_t> m_RegisteredIOs;
    std::set<uint32_t> m_RegisteredLinks;

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

    NodeMap &GetNodeMap() { return m_NodeMap; }
    LinkMap &GetLinkMap() { return m_LinkMap; }
    std::set<uint32_t> &GetNodes() { return m_RegisteredNodes; }
    std::set<uint32_t> &GetIOs() { return m_RegisteredIOs; }
    std::set<uint32_t> &GetLinks() { return m_RegisteredLinks; }
};
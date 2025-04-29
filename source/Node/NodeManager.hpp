#pragma once

#include "NodeTypes.hpp"

class NodeManager
{
public:
    NodeManager() = default;
    ~NodeManager() = default;

    void Render();
    void Update();

    void CreateNode();
    void CreateNode(const ImVec2& position);
    void CreateNode(const std::string& name);
    void CreateNode(const std::string& name, const ImVec2& position);
    void CreateLink(uint32_t pin1Id, uint32_t pin2Id);
    void CreatePin(uint32_t nodeId, PinType pinType);
    void CreatePin(uint32_t nodeId, PinType pinType, const std::string& name);

    void RemoveNode(uint32_t nodeId);
    void RemoveNodes(const NodeIDSet& nodeIds);
    void RemoveLink(uint32_t linkId);
    void RemoveLinks(const LinkIDSet& linkIds);
    void RemovePin(uint32_t nodeId, uint32_t pinId);

    const NodeIDSet& GetRegisteredNodes() const;
    const NodeData& GetNodeData(uint32_t nodeId) const;
    const PinData& GetPinData(uint32_t pinId) const;
    const LinkIDSet& GetRegisteredLinks() const;
    const LinkData& GetLinkData(uint32_t linkId) const;

private:
    void RegisterNode(uint32_t nodeId);
    void RegisterPin(uint32_t pinId);
    void RegisterLink(uint32_t linkId);

    void DeregisterNode(uint32_t nodeId);
    void DeregisterPin(uint32_t pinId);
    void DeregisterLink(uint32_t linkId);

    void ProcessQueues();
    void ProcessNodeQueues();
    void ProcessPinQueues();
    void ProcessLinkQueues();

    uint32_t GetNewNodeID();
    uint32_t GetNewPinID();
    uint32_t GetNewLinkID();

    void RenderState();

private:
    uint32_t m_NextNodeID = 0;
    uint32_t m_NextPinID = 0;
    uint32_t m_NextLinkID = 0;

    NodeDataMap m_NodeDataMap;
    PinDataMap m_PinDataMap;
    LinkDataMap m_LinkDataMap;

    NodeIDQueue m_NodesToRegister;
    NodeIDQueue m_NodesToDeregister;
    NodeIDSet m_RegisteredNodes;

    PinIDQueue m_PinsToRegister;
    PinIDQueue m_PinsToDeregister;
    PinIDSet m_RegisteredPins;

    LinkIDQueue m_LinksToRegister;
    LinkIDQueue m_LinksToDeregister;
    LinkIDSet m_RegisteredLinks;
};
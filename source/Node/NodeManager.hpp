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
    void CreateNode(float x, float y);
    void CreateNode(const std::string& name);
    void CreateNode(const std::string& name, float x, float y);
    void CreateLink(std::uint32_t pin1Id, std::uint32_t pin2Id);
    void CreatePin(std::uint32_t nodeId, PinType pinType);
    void CreatePin(std::uint32_t nodeId, PinType pinType, const std::string& name);

    void RemoveNode(std::uint32_t nodeId);
    void RemoveNodes(const IDSet& nodeIds);
    void RemoveLink(std::uint32_t linkId);
    void RemoveLinks(const IDSet& linkIds);
    void RemovePin(std::uint32_t nodeId, std::uint32_t pinId);

    inline void SetNodeColor(std::uint32_t nodeId, ColorData colors) { m_NodeDataMap[nodeId].Colors = colors; }
    inline void SetLinkColor(std::uint32_t linkId, ColorData colors) { m_LinkDataMap[linkId].Colors = colors; }
    inline void SetPinColor(std::uint32_t pinId, ColorData colors) { m_PinDataMap[pinId].Colors = colors; }

    inline const IDSet& GetRegisteredNodes() const { return m_RegisteredNodes; }
    inline const NodeData& GetNodeData(std::uint32_t nodeId) const { return m_NodeDataMap.at(nodeId); }

    inline const IDSet& GetRegisteredPins() const { return m_RegisteredPins; }
    inline const PinData& GetPinData(std::uint32_t pinId) const { return m_PinDataMap.at(pinId); }

    inline const IDSet& GetRegisteredLinks() const { return m_RegisteredLinks; }
    inline const LinkData& GetLinkData(std::uint32_t linkId) const { return m_LinkDataMap.at(linkId); }

private:
    void RegisterNode(std::uint32_t nodeId);
    void RegisterPin(std::uint32_t pinId);
    void RegisterLink(std::uint32_t linkId);

    void DeregisterNode(std::uint32_t nodeId);
    void DeregisterPin(std::uint32_t pinId);
    void DeregisterLink(std::uint32_t linkId);

    void ProcessQueues();
    void ProcessNodeQueues();
    void ProcessPinQueues();
    void ProcessLinkQueues();

    inline std::uint32_t GetNewNodeID() { return m_NextNodeID++; }
    inline std::uint32_t GetNewPinID() { return m_NextPinID++; }
    inline std::uint32_t GetNewLinkID() { return m_NextLinkID++; }

private:
    std::uint32_t m_NextNodeID = 0;
    std::uint32_t m_NextPinID = 0;
    std::uint32_t m_NextLinkID = 0;

    IDQueue m_NodesToRegister;
    IDQueue m_NodesToDeregister;
    IDSet m_RegisteredNodes;
    NodeDataMap m_NodeDataMap;

    IDQueue m_PinsToRegister;
    IDQueue m_PinsToDeregister;
    IDSet m_RegisteredPins;
    PinDataMap m_PinDataMap;

    IDQueue m_LinksToRegister;
    IDQueue m_LinksToDeregister;
    IDSet m_RegisteredLinks;
    LinkDataMap m_LinkDataMap;
};
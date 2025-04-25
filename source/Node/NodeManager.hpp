#pragma once

#include <imgui.h>

#include <cstdint>
#include <queue>
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>

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
    uint32_t NodeID;
    PinType PinType;
    std::string Name;
};

struct NodeData
{
    std::string Name;
    PinIDSet InputIDs;
    PinIDSet OutputIDs;
    LinkIDSet LinkIDs;
};

struct LinkData
{
    uint32_t Node1ID;
    uint32_t Pin1ID;
    uint32_t Node2ID;
    uint32_t Pin2ID;
};

using PinDataMap = std::unordered_map<uint32_t, PinData>;
using NodeDataMap = std::unordered_map<uint32_t, NodeData>;
using LinkDataMap = std::unordered_map<uint32_t, LinkData>;

class NodeManager
{
public:
    NodeManager() = default;
    ~NodeManager() = default;

    void CreateNode()
    {
        uint32_t nodeId = GetNewNodeID();
        std::string name = "Node (ID: " + std::to_string(nodeId) + ")";

        std::cout << "[NodeManager] Adding node id " << nodeId << " to register queue" << std::endl;
        m_NodesToRegister.push(nodeId);

        std::cout << "[NodeManager] Adding node id " << nodeId << " data to node data map" << std::endl;
        m_NodeDataMap[nodeId] = NodeData{name};
    }

    void CreateNode(const std::string &name)
    {
        uint32_t nodeId = GetNewNodeID();

        std::cout << "[NodeManager] Adding node id " << nodeId << " to register queue" << std::endl;
        m_NodesToRegister.push(nodeId);

        std::cout << "[NodeManager] Adding node id " << nodeId << " data to node data map" << std::endl;
        m_NodeDataMap[nodeId] = NodeData{name};
    }

    void CreateLink(uint32_t node1, uint32_t node2)
    {
        uint32_t linkId = GetNewLinkID();

        std::cout << "[NodeManager] Adding link id " << linkId << " to register queue" << std::endl;
        m_LinksToRegister.push(linkId);

        std::cout << "[NodeManager] Adding link id " << linkId << " data to link data map" << std::endl;
        m_LinkDataMap[linkId] = LinkData{node1, node2};
    }

    void CreatePin(uint32_t nodeId, PinType pinType)
    {
        uint32_t pinId = GetNewPinID();

        std::string name = "Pin (ID: " + std::to_string(pinId) + ")";

        // Check if node ID is registered
        if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
        {
            std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
            return;
        }

        // Find node data and add pin to either inputs or outputs
        auto &nodeData = m_NodeDataMap.at(nodeId);
        if (pinType == PinType::Input)
        {
            nodeData.InputIDs.insert(pinId);
        }
        else
        {
            nodeData.OutputIDs.insert(pinId);
        }

        std::cout << "[NodeManager] Adding pin id " << pinId << " to register queue" << std::endl;
        m_PinsToRegister.push(pinId);

        std::cout << "[NodeManager] Adding pin id " << pinId << " data to pin data map" << std::endl;
        m_PinDataMap[pinId] = PinData{nodeId, pinType, name};
    }

    void RemoveNode(uint32_t nodeId)
    {
        // Check if node ID is registered
        if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
        {
            std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
            return;
        }

        // Clear all pins from node
        auto &nodeData = m_NodeDataMap.at(nodeId);
        for (const auto &inputId : nodeData.InputIDs)
        {
            RemovePin(nodeId, inputId);
        }

        for (const auto &outputId : nodeData.OutputIDs)
        {
            RemovePin(nodeId, outputId);
        }

        std::cout << "[NodeManager] Adding node id " << nodeId << " to deregister queue" << std::endl;
        m_NodesToDeregister.push(nodeId);
    }

    void RemoveNodes(const NodeIDSet &nodeIds)
    {
        for (const auto &nodeId : nodeIds)
        {
            RemoveNode(nodeId);
        }
    }

    void RemoveLink(uint32_t linkId)
    {
        // Check if link ID is registered
        if (m_RegisteredLinks.find(linkId) == m_RegisteredLinks.end())
        {
            std::cerr << "[NodeManager] Link not registered: " << linkId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding link id " << linkId << " to deregister queue" << std::endl;
        m_LinksToDeregister.push(linkId);
    }

    void RemoveLinks(const LinkIDSet &linkIds)
    {
        for (const auto &linkId : linkIds)
        {
            RemoveLink(linkId);
        }
    }

    void RemovePin(uint32_t nodeId, uint32_t pinId)
    {
        // Check if pin ID is registered
        if (m_RegisteredPins.find(pinId) == m_RegisteredPins.end())
        {
            std::cerr << "[NodeManager] Pin not registered: " << pinId << std::endl;
            return;
        }

        // Check if node ID is registered
        if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
        {
            std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
            return;
        }

        // Remove pin data from node data map
        m_NodeDataMap[nodeId].InputIDs.erase(pinId);
        m_NodeDataMap[nodeId].OutputIDs.erase(pinId);

        std::cout << "[NodeManager] Adding pin id " << pinId << " to deregister queue" << std::endl;
        m_PinsToDeregister.push(pinId);
    }

    void CreatePin(uint32_t nodeId, PinType pinType, const std::string &name)
    {
        uint32_t pinId = GetNewPinID();

        // Check if node ID is registered
        if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
        {
            std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding pin id " << pinId << " to register queue" << std::endl;
        m_PinsToRegister.push(pinId);

        std::cout << "[NodeManager] Adding pin id " << pinId << " data to pin data map" << std::endl;
        m_PinDataMap[pinId] = PinData{nodeId, pinType, name};
    }

    void RegisterNode(uint32_t nodeId)
    {
        // Check if node ID is already registered
        if (m_RegisteredNodes.find(nodeId) != m_RegisteredNodes.end())
        {
            std::cerr << "[NodeManager] Node already registered: " << nodeId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding node id " << nodeId << " to register queue" << std::endl;
        m_NodesToRegister.push(nodeId);
    }

    void RegisterPin(uint32_t pinId)
    {
        // Check if pin ID is already registered
        if (m_RegisteredPins.find(pinId) != m_RegisteredPins.end())
        {
            std::cerr << "[NodeManager] Pin already registered: " << pinId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding pin id " << pinId << " to register queue" << std::endl;
        m_PinsToRegister.push(pinId);
    }

    void RegisterLink(uint32_t linkId)
    {
        // Check if link ID is already registered
        if (m_RegisteredLinks.find(linkId) != m_RegisteredLinks.end())
        {
            std::cerr << "[NodeManager] Link already registered: " << linkId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding link id " << linkId << " to register queue" << std::endl;
        m_LinksToRegister.push(linkId);
    }

    void DeregisterNode(uint32_t nodeId)
    {
        // Check if node ID is registered
        if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
        {
            std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding node id " << nodeId << " to deregister queue" << std::endl;
        m_NodesToDeregister.push(nodeId);
    }

    void DeregisterPin(uint32_t pinId)
    {
        // Check if pin ID is registered
        if (m_RegisteredPins.find(pinId) == m_RegisteredPins.end())
        {
            std::cerr << "[NodeManager] Pin not registered: " << pinId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding pin id " << pinId << " to deregister queue" << std::endl;
        m_PinsToDeregister.push(pinId);
    }

    void DeregisterLink(uint32_t linkId)
    {
        // Check if link ID is registered
        if (m_RegisteredLinks.find(linkId) == m_RegisteredLinks.end())
        {
            std::cerr << "[NodeManager] Link not registered: " << linkId << std::endl;
            return;
        }

        std::cout << "[NodeManager] Adding link id " << linkId << " to deregister queue" << std::endl;
        m_LinksToDeregister.push(linkId);
    }

    void ProcessQueues()
    {
        ProcessNodeQueues();
        ProcessPinQueues();
        ProcessLinkQueues();
    }

    void ProcessNodeQueues()
    {
        while (!m_NodesToRegister.empty())
        {
            uint32_t nodeId = m_NodesToRegister.front();
            m_NodesToRegister.pop();

            std::cout << "[NodeManager] Registering node " << nodeId << std::endl;
            m_RegisteredNodes.insert(nodeId);
        }

        while (!m_NodesToDeregister.empty())
        {
            uint32_t nodeId = m_NodesToDeregister.front();
            m_NodesToDeregister.pop();

            std::cout << "[NodeManager] Deregistering node " << nodeId << std::endl;
            m_RegisteredNodes.erase(nodeId);

            // Remove node data from node data map
            m_NodeDataMap.erase(nodeId);
        }
    }

    void ProcessPinQueues()
    {
        while (!m_PinsToRegister.empty())
        {
            uint32_t pinId = m_PinsToRegister.front();
            m_PinsToRegister.pop();

            std::cout << "[NodeManager] Registering pin " << pinId << std::endl;
            m_RegisteredPins.insert(pinId);
        }

        while (!m_PinsToDeregister.empty())
        {
            uint32_t pinId = m_PinsToDeregister.front();
            m_PinsToDeregister.pop();

            std::cout << "[NodeManager] Deregistering pin " << pinId << std::endl;
            m_RegisteredPins.erase(pinId);

            // Remove pin data from pin data map
            m_PinDataMap.erase(pinId);
        }
    }

    void ProcessLinkQueues()
    {
        while (!m_LinksToRegister.empty())
        {
            uint32_t linkId = m_LinksToRegister.front();
            m_LinksToRegister.pop();

            std::cout << "[NodeManager] Registering link " << linkId << std::endl;
            m_RegisteredLinks.insert(linkId);
        }

        while (!m_LinksToDeregister.empty())
        {
            uint32_t linkId = m_LinksToDeregister.front();
            m_LinksToDeregister.pop();

            std::cout << "[NodeManager] Deregistering link " << linkId << std::endl;
            m_RegisteredLinks.erase(linkId);

            // Remove link data from link data map
            m_LinkDataMap.erase(linkId);
        }
    }

    uint32_t GetNewNodeID()
    {
        return m_NextNodeID++;
    }
    uint32_t GetNewPinID()
    {
        return m_NextPinID++;
    }
    uint32_t GetNewLinkID()
    {
        return m_NextLinkID++;
    }

    const NodeIDSet &GetRegisteredNodes() const
    {
        return m_RegisteredNodes;
    }

    const NodeData &GetNodeData(uint32_t nodeId) const
    {
        return m_NodeDataMap.at(nodeId);
    }

    const PinData &GetPinData(uint32_t pinId) const
    {
        return m_PinDataMap.at(pinId);
    }

    const LinkData &GetLinkData(uint32_t linkId) const
    {
        return m_LinkDataMap.at(linkId);
    }

    void RenderState()
    {
        ImGui::Begin("Node Manager State");
        ImGui::Text("Registered Nodes: %d", m_RegisteredNodes.size());
        ImGui::Text("Registered Pins: %d", m_RegisteredPins.size());
        ImGui::Text("Registered Links: %d", m_RegisteredLinks.size());

        ImGui::Separator();

        // Node Data
        ImGui::Text("Node Data");
        for (const auto &node : m_NodeDataMap)
        {
            ImGui::Text("Node ID: %d", node.first);
            ImGui::Text("Node Name: %s", node.second.Name.c_str());
            for (const auto &input : node.second.InputIDs)
            {
                ImGui::Text("Input ID: %d", input);
            }
            for (const auto &output : node.second.OutputIDs)
            {
                ImGui::Text("Output ID: %d", output);
            }
        }

        ImGui::End();
    }

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
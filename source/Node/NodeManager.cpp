#include "NodeManager.hpp"

#include <iostream>

void NodeManager::Update()
{
    ProcessQueues();
}

void NodeManager::CreateNode()
{
    std::uint32_t nodeId = GetNewNodeID();
    std::string name = "Node (ID: " + std::to_string(nodeId) + ")";
    m_NodesToRegister.push(nodeId);
    m_NodeDataMap[nodeId] = NodeData{Name : name};
}

void NodeManager::CreateNode(float x, float y)
{
    std::uint32_t nodeId = GetNewNodeID();
    std::string name = "Node (ID: " + std::to_string(nodeId) + ")";
    m_NodesToRegister.push(nodeId);
    m_NodeDataMap[nodeId] = NodeData{Name : name, Position : {x, y}};
}

void NodeManager::CreateNode(const std::string& name)
{
    std::uint32_t nodeId = GetNewNodeID();
    m_NodesToRegister.push(nodeId);
    m_NodeDataMap[nodeId] = NodeData{Name : name};
}

void NodeManager::CreateNode(const std::string& name, float x, float y)
{
    std::uint32_t nodeId = GetNewNodeID();
    m_NodesToRegister.push(nodeId);
    m_NodeDataMap[nodeId] = NodeData{Name : name, Position : {x, y}};
}

void NodeManager::CreateLink(std::uint32_t pin1Id, std::uint32_t pin2Id)
{
    std::uint32_t linkId = GetNewLinkID();

    // Check if pin1 ID is registered
    if (m_RegisteredPins.find(pin1Id) == m_RegisteredPins.end())
    {
        return;
    }

    // Check if pin2 ID is registered
    if (m_RegisteredPins.find(pin2Id) == m_RegisteredPins.end())
    {
        return;
    }

    // Get pin data for pin1 and pin2
    auto& pin1Data = m_PinDataMap.at(pin1Id);
    auto& pin2Data = m_PinDataMap.at(pin2Id);

    // Add link to pin1 and pin2
    pin1Data.Links.insert(linkId);
    pin2Data.Links.insert(linkId);

    m_LinksToRegister.push(linkId);
    m_LinkDataMap[linkId] = LinkData{Pin1ID : pin1Id, Pin2ID : pin2Id};
}

void NodeManager::CreatePin(std::uint32_t nodeId, PinType pinType)
{
    std::uint32_t pinId = GetNewPinID();
    std::string name = "Pin (ID: " + std::to_string(pinId) + ")";

    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        return;
    }

    // Find node data and add pin to either inputs or outputs
    auto& nodeData = m_NodeDataMap.at(nodeId);
    if (pinType == PinType::Input)
    {
        nodeData.InputIDs.insert(pinId);
    }
    else
    {
        nodeData.OutputIDs.insert(pinId);
    }

    m_PinsToRegister.push(pinId);
    m_PinDataMap[pinId] = PinData{Name : name, NodeID : nodeId, Type : pinType};
}

void NodeManager::RemoveNode(std::uint32_t nodeId)
{
    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        return;
    }

    // Clear all pins from node
    auto& nodeData = m_NodeDataMap.at(nodeId);
    for (const auto& inputId : nodeData.InputIDs)
    {
        auto& pinData = m_PinDataMap.at(inputId);
        if (pinData.Links.size() > 0)
        {
            for (const auto& linkId : pinData.Links)
            {
                RemoveLink(linkId);
            }
        }
        RemovePin(nodeId, inputId);
    }

    for (const auto& outputId : nodeData.OutputIDs)
    {
        auto& pinData = m_PinDataMap.at(outputId);
        if (pinData.Links.size() > 0)
        {
            for (const auto& linkId : pinData.Links)
            {
                RemoveLink(linkId);
            }
        }
        RemovePin(nodeId, outputId);
    }

    m_NodesToDeregister.push(nodeId);
}

void NodeManager::RemoveNodes(const IDSet& nodeIds)
{
    for (const auto& nodeId : nodeIds)
    {
        RemoveNode(nodeId);
    }
}

void NodeManager::RemoveLink(std::uint32_t linkId)
{
    // Check if link ID is registered
    if (m_RegisteredLinks.find(linkId) == m_RegisteredLinks.end())
    {
        return;
    }

    m_LinksToDeregister.push(linkId);
}

void NodeManager::RemoveLinks(const IDSet& linkIds)
{
    for (const auto& linkId : linkIds)
    {
        RemoveLink(linkId);
    }
}

void NodeManager::RemovePin(std::uint32_t nodeId, std::uint32_t pinId)
{
    // Check if pin ID is registered
    if (m_RegisteredPins.find(pinId) == m_RegisteredPins.end())
    {
        return;
    }

    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        return;
    }

    // Remove pin data from node data map
    m_NodeDataMap[nodeId].InputIDs.erase(pinId);
    m_NodeDataMap[nodeId].OutputIDs.erase(pinId);

    m_PinsToDeregister.push(pinId);
}

void NodeManager::CreatePin(std::uint32_t nodeId, PinType pinType, const std::string& name)
{
    std::uint32_t pinId = GetNewPinID();

    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        return;
    }

    m_PinsToRegister.push(pinId);
    m_PinDataMap[pinId] = PinData{Name : name, NodeID : nodeId, Type : pinType};
}

void NodeManager::RegisterNode(std::uint32_t nodeId)
{
    // Check if node ID is already registered
    if (m_RegisteredNodes.find(nodeId) != m_RegisteredNodes.end())
    {
        return;
    }

    m_NodesToRegister.push(nodeId);
}

void NodeManager::RegisterPin(std::uint32_t pinId)
{
    // Check if pin ID is already registered
    if (m_RegisteredPins.find(pinId) != m_RegisteredPins.end())
    {
        return;
    }

    m_PinsToRegister.push(pinId);
}

void NodeManager::RegisterLink(std::uint32_t linkId)
{
    // Check if link ID is already registered
    if (m_RegisteredLinks.find(linkId) != m_RegisteredLinks.end())
    {
        return;
    }

    m_LinksToRegister.push(linkId);
}

void NodeManager::DeregisterNode(std::uint32_t nodeId)
{
    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        return;
    }

    m_NodesToDeregister.push(nodeId);
}

void NodeManager::DeregisterPin(std::uint32_t pinId)
{
    // Check if pin ID is registered
    if (m_RegisteredPins.find(pinId) == m_RegisteredPins.end())
    {
        return;
    }

    m_PinsToDeregister.push(pinId);
}

void NodeManager::DeregisterLink(std::uint32_t linkId)
{
    // Check if link ID is registered
    if (m_RegisteredLinks.find(linkId) == m_RegisteredLinks.end())
    {
        return;
    }

    m_LinksToDeregister.push(linkId);
}

void NodeManager::ProcessQueues()
{
    ProcessNodeQueues();
    ProcessPinQueues();
    ProcessLinkQueues();
}

void NodeManager::ProcessNodeQueues()
{
    while (!m_NodesToRegister.empty())
    {
        std::uint32_t nodeId = m_NodesToRegister.front();

        m_NodesToRegister.pop();

        m_RegisteredNodes.insert(nodeId);
    }

    while (!m_NodesToDeregister.empty())
    {
        std::uint32_t nodeId = m_NodesToDeregister.front();

        m_NodesToDeregister.pop();

        m_RegisteredNodes.erase(nodeId);

        // Remove node data from node data map
        m_NodeDataMap.erase(nodeId);
    }
}

void NodeManager::ProcessPinQueues()
{
    while (!m_PinsToRegister.empty())
    {
        std::uint32_t pinId = m_PinsToRegister.front();

        m_PinsToRegister.pop();

        m_RegisteredPins.insert(pinId);
    }

    while (!m_PinsToDeregister.empty())
    {
        std::uint32_t pinId = m_PinsToDeregister.front();

        m_PinsToDeregister.pop();

        m_RegisteredPins.erase(pinId);

        // Remove pin data from pin data map
        m_PinDataMap.erase(pinId);
    }
}

void NodeManager::ProcessLinkQueues()
{
    while (!m_LinksToRegister.empty())
    {
        std::uint32_t linkId = m_LinksToRegister.front();

        m_LinksToRegister.pop();

        m_RegisteredLinks.insert(linkId);
    }

    while (!m_LinksToDeregister.empty())
    {
        std::uint32_t linkId = m_LinksToDeregister.front();

        m_LinksToDeregister.pop();

        m_RegisteredLinks.erase(linkId);

        // Remove link data from link data map
        m_LinkDataMap.erase(linkId);
    }
}

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

    std::cout << "[NodeManager] Adding node id " << nodeId << " to register queue" << std::endl;
    m_NodesToRegister.push(nodeId);

    std::cout << "[NodeManager] Adding node id " << nodeId << " data to node data map" << std::endl;
    m_NodeDataMap[nodeId] = NodeData{Name : name};
}

void NodeManager::CreateNode(float x, float y)
{
    std::uint32_t nodeId = GetNewNodeID();
    std::string name = "Node (ID: " + std::to_string(nodeId) + ")";

    std::cout << "[NodeManager] Adding node id " << nodeId << " to register queue" << std::endl;
    m_NodesToRegister.push(nodeId);

    std::cout << "[NodeManager] Adding node id " << nodeId << " data to node data map" << std::endl;
    m_NodeDataMap[nodeId] = NodeData{Name : name, Position : std::make_pair(x, y)};
}

void NodeManager::CreateNode(const std::string& name)
{
    std::uint32_t nodeId = GetNewNodeID();
    m_NodesToRegister.push(nodeId);

    std::cout << "[NodeManager] Adding node id " << nodeId << " data to node data map" << std::endl;
    m_NodeDataMap[nodeId] = NodeData{Name : name};
}

void NodeManager::CreateNode(const std::string& name, float x, float y)
{
    uint32_t nodeId = GetNewNodeID();

    std::cout << "[NodeManager] Adding node id " << nodeId << " to register queue" << std::endl;
    m_NodesToRegister.push(nodeId);

    std::cout << "[NodeManager] Adding node id " << nodeId << " data to node data map" << std::endl;
    m_NodeDataMap[nodeId] = NodeData{Name : name, Position : std::make_pair(x, y)};
}

void NodeManager::CreateLink(uint32_t pin1Id, uint32_t pin2Id)
{
    std::uint32_t linkId = GetNewLinkID();

    // Check if pin1 ID is registered
    if (m_RegisteredPins.find(pin1Id) == m_RegisteredPins.end())
    {
        std::cerr << "[NodeManager] Pin not registered: " << pin1Id << std::endl;
        return;
    }

    // Check if pin2 ID is registered
    if (m_RegisteredPins.find(pin2Id) == m_RegisteredPins.end())
    {
        std::cerr << "[NodeManager] Pin not registered: " << pin2Id << std::endl;
        return;
    }

    // Get pin data for pin1 and pin2
    auto& pin1Data = m_PinDataMap.at(pin1Id);
    auto& pin2Data = m_PinDataMap.at(pin2Id);

    // Add link to pin1 and pin2
    pin1Data.Links.insert(linkId);
    pin2Data.Links.insert(linkId);

    std::cout << "[NodeManager] Adding link id " << linkId << " to register queue" << std::endl;
    m_LinksToRegister.push(linkId);

    std::cout << "[NodeManager] Adding link id " << linkId << " data to link data map" << std::endl;
    m_LinkDataMap[linkId] = LinkData{Pin1ID : pin1Id, Pin2ID : pin2Id};
}

void NodeManager::CreatePin(std::uint32_t nodeId, PinType pinType)
{
    std::uint32_t pinId = GetNewPinID();
    std::string name = "Pin (ID: " + std::to_string(pinId) + ")";

    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
        return;
    }

    // Find node data and add pin to either inputs or outputs
    auto& nodeData = m_NodeDataMap.at(nodeId);
    if (pinType == PinType::Input)
    {
        std::cout << "[NodeManager] Adding pin id " << pinId << " to node id " << nodeId << " as input" << std::endl;
        nodeData.InputIDs.insert(pinId);
    }
    else
    {
        std::cout << "[NodeManager] Adding pin id " << pinId << " to node id " << nodeId << " as output" << std::endl;
        nodeData.OutputIDs.insert(pinId);
    }

    std::cout << "[NodeManager] Adding pin id " << pinId << " to register queue" << std::endl;
    m_PinsToRegister.push(pinId);

    std::cout << "[NodeManager] Adding pin id " << pinId << " data to pin data map" << std::endl;
    m_PinDataMap[pinId] = PinData{Name : name, NodeID : nodeId, Type : pinType};
}

void NodeManager::RemoveNode(std::uint32_t nodeId)
{
    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
        return;
    }

    // Clear all pins from node
    auto& nodeData = m_NodeDataMap.at(nodeId);
    for (const auto& inputId : nodeData.InputIDs)
    {
        std::cout << "[NodeManager] Removing pin id " << inputId << " from node id " << nodeId << " as input"
                  << std::endl;

        auto& pinData = m_PinDataMap.at(inputId);
        if (pinData.Links.size() > 0)
        {
            std::cout << "[NodeManager] Pin has links: " << inputId << std::endl;
            for (const auto& linkId : pinData.Links)
            {
                std::cout << "[NodeManager] Removing link id " << linkId << " from pin id " << inputId << std::endl;
                RemoveLink(linkId);
            }
        }

        RemovePin(nodeId, inputId);
    }

    for (const auto& outputId : nodeData.OutputIDs)
    {
        std::cout << "[NodeManager] Removing pin id " << outputId << " from node id " << nodeId << " as output"
                  << std::endl;

        auto& pinData = m_PinDataMap.at(outputId);
        if (pinData.Links.size() > 0)
        {
            std::cout << "[NodeManager] Pin has links: " << outputId << std::endl;
            for (const auto& linkId : pinData.Links)
            {
                std::cout << "[NodeManager] Removing link id " << linkId << " from pin id " << outputId << std::endl;
                RemoveLink(linkId);
            }
        }

        RemovePin(nodeId, outputId);
    }

    std::cout << "[NodeManager] Adding node id " << nodeId << " to deregister queue" << std::endl;
    m_NodesToDeregister.push(nodeId);
}

void NodeManager::RemoveNodes(const IDSet& nodeIds)
{
    for (const auto& nodeId : nodeIds)
    {
        std::cout << "[NodeManager] Removing node id " << nodeId << " from deregister queue" << std::endl;
        RemoveNode(nodeId);
    }
}

void NodeManager::RemoveLink(std::uint32_t linkId)
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

void NodeManager::RemoveLinks(const IDSet& linkIds)
{
    for (const auto& linkId : linkIds)
    {
        std::cout << "[NodeManager] Removing link id " << linkId << " from deregister queue" << std::endl;
        RemoveLink(linkId);
    }
}

void NodeManager::RemovePin(std::uint32_t nodeId, std::uint32_t pinId)
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

void NodeManager::CreatePin(std::uint32_t nodeId, PinType pinType, const std::string& name)
{
    std::uint32_t pinId = GetNewPinID();

    // Check if node ID is registered
    if (m_RegisteredNodes.find(nodeId) == m_RegisteredNodes.end())
    {
        std::cerr << "[NodeManager] Node not registered: " << nodeId << std::endl;
        return;
    }

    std::cout << "[NodeManager] Adding pin id " << pinId << " to register queue" << std::endl;
    m_PinsToRegister.push(pinId);
    m_PinDataMap[pinId] = PinData{Name : name, NodeID : nodeId, Type : pinType};
}

void NodeManager::RegisterNode(std::uint32_t nodeId)
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

void NodeManager::RegisterPin(std::uint32_t pinId)
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

void NodeManager::RegisterLink(std::uint32_t linkId)
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

void NodeManager::DeregisterNode(std::uint32_t nodeId)
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

void NodeManager::DeregisterPin(std::uint32_t pinId)
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

void NodeManager::DeregisterLink(std::uint32_t linkId)
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

        std::cout << "[NodeManager] Removing node id " << nodeId << " from register queue" << std::endl;
        m_NodesToRegister.pop();

        std::cout << "[NodeManager] Registering node " << nodeId << std::endl;
        m_RegisteredNodes.insert(nodeId);
    }

    while (!m_NodesToDeregister.empty())
    {
        std::uint32_t nodeId = m_NodesToDeregister.front();

        std::cout << "[NodeManager] Removing node id " << nodeId << " from deregister queue" << std::endl;
        m_NodesToDeregister.pop();

        std::cout << "[NodeManager] Deregistering node " << nodeId << std::endl;
        m_RegisteredNodes.erase(nodeId);

        // Remove node data from node data map
        std::cout << "[NodeManager] Removing node id " << nodeId << " data from node data map" << std::endl;
        m_NodeDataMap.erase(nodeId);
    }
}

void NodeManager::ProcessPinQueues()
{
    while (!m_PinsToRegister.empty())
    {
        std::uint32_t pinId = m_PinsToRegister.front();

        std::cout << "[NodeManager] Removing pin id " << pinId << " from register queue" << std::endl;
        m_PinsToRegister.pop();

        std::cout << "[NodeManager] Registering pin " << pinId << std::endl;
        m_RegisteredPins.insert(pinId);
    }

    while (!m_PinsToDeregister.empty())
    {
        std::uint32_t pinId = m_PinsToDeregister.front();

        std::cout << "[NodeManager] Removing pin id " << pinId << " from deregister queue" << std::endl;
        m_PinsToDeregister.pop();

        std::cout << "[NodeManager] Deregistering pin " << pinId << std::endl;
        m_RegisteredPins.erase(pinId);

        // Remove pin data from pin data map
        std::cout << "[NodeManager] Removing pin id " << pinId << " data from pin data map" << std::endl;
        m_PinDataMap.erase(pinId);
    }
}

void NodeManager::ProcessLinkQueues()
{
    while (!m_LinksToRegister.empty())
    {
        std::uint32_t linkId = m_LinksToRegister.front();

        std::cout << "[NodeManager] Removing link id " << linkId << " from register queue" << std::endl;
        m_LinksToRegister.pop();

        std::cout << "[NodeManager] Registering link " << linkId << std::endl;
        m_RegisteredLinks.insert(linkId);
    }

    while (!m_LinksToDeregister.empty())
    {
        std::uint32_t linkId = m_LinksToDeregister.front();

        std::cout << "[NodeManager] Removing link id " << linkId << " from deregister queue" << std::endl;
        m_LinksToDeregister.pop();

        std::cout << "[NodeManager] Deregistering link " << linkId << std::endl;
        m_RegisteredLinks.erase(linkId);

        // Remove link data from link data map
        std::cout << "[NodeManager] Removing link id " << linkId << " data from link data map" << std::endl;
        m_LinkDataMap.erase(linkId);
    }
}

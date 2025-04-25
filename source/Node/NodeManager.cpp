#include "NodeManager.hpp"

#include <imnodes.h>
#include <imgui.h>

#include <memory>
#include <iostream>

NodeManager::NodeManager()
{
}

void NodeManager::CreateNode()
{
    std::string name = "Node (ID " + std::to_string(m_NextNodeId) + ")";
    CreateNode(name);
}

void NodeManager::CreateNode(const std::string &name)
{
    // Create Node
    NodePtr node = std::make_shared<Node>();
    node->ID = m_NextNodeId++;
    node->Name = name;

    // Add to map
    m_Nodes[node->ID] = node;
    std::cout << "Created node " << node->ID << std::endl;
}

void NodeManager::RemoveNode(uint32_t nodeId)
{
    NodePtr node = m_Nodes[nodeId];

    // Remove all inputs
    for (auto &input : node->Inputs)
    {
        m_IOs.erase(input.ID);
    }

    // Remove all outputs
    for (auto &output : node->Outputs)
    {
        m_IOs.erase(output.ID);
    }

    // Remove all links
    for (auto &linkId : node->LinkIDs)
    {
        RemoveLink(linkId);
    }

    // Ensure key exists
    if (m_Nodes.find(nodeId) == m_Nodes.end())
    {
        std::cerr << "[NodeManager] Node " << nodeId << " does not exist" << std::endl;
        return;
    }

    m_Nodes.erase(nodeId);
    std::cout << "Removed node " << nodeId << std::endl;
}

uint32_t NodeManager::AddInput(uint32_t nodeId)
{
    std::string name = "Input (ID " + std::to_string(m_NextIOId) + ")";
    NodeIO input = NodeIO(nodeId, m_NextIOId, name);

    m_Nodes[nodeId]->Inputs.push_back(input);
    m_IOs.insert(m_NextIOId);

    m_NextIOId++;
    return m_NextIOId - 1;
}

uint32_t NodeManager::AddInput(uint32_t nodeId, const std::string &name)
{
    NodeIO input = NodeIO(nodeId, m_NextIOId, name);

    m_Nodes[nodeId]->Inputs.push_back(input);
    m_IOs.insert(m_NextIOId);

    m_NextIOId++;
    return m_NextIOId - 1;
}

void NodeManager::RenameInput(uint32_t nodeId, uint32_t inputId, const std::string &name)
{
    NodePtr node = m_Nodes[nodeId];
    for (auto &input : node->Inputs)
    {
        if (input.ID == inputId)
        {
            input.Name = name;
        }
    }
}

void NodeManager::RemoveInput(uint32_t nodeId, uint32_t inputId)
{
    NodePtr node = m_Nodes[nodeId];

    // Search through links for the input id
    for (auto &[linkId, inputOutput] : m_Links)
    {
        if (inputOutput.first == inputId)
        {
            RemoveLink(linkId);
        }
    }
}

uint32_t NodeManager::AddOutput(uint32_t nodeId)
{
    if (m_Nodes.find(nodeId) == m_Nodes.end())
    {
        std::cerr << "[NodeManager] Node " << nodeId << " does not exist" << std::endl;
        return 0;
    }

    std::string name = "Output (ID " + std::to_string(m_NextIOId) + ")";
    NodeIO output = NodeIO(nodeId, m_NextIOId, name);
    m_Nodes[nodeId]->Outputs.push_back(output);
    m_IOs.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
}

uint32_t NodeManager::AddOutput(uint32_t nodeId, const std::string &name)
{
    NodeIO output = NodeIO(nodeId, m_NextIOId, name);
    m_Nodes[nodeId]->Outputs.push_back(output);
    m_IOs.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
}

void NodeManager::RenameOutput(uint32_t nodeId, uint32_t outputId, const std::string &name)
{
    NodePtr node = m_Nodes[nodeId];
    assert(node != nullptr);
    for (auto &output : node->Outputs)
    {
        if (output.ID == outputId)
        {
            output.Name = name;
        }
    }
}

void NodeManager::RemoveOutput(uint32_t nodeId, uint32_t outputId)
{
    NodePtr node = m_Nodes[nodeId];

    // Search through links for the output id
    for (auto &[linkId, inputOutput] : m_Links)
    {
        if (inputOutput.second == outputId)
        {
            RemoveLink(linkId);
        }
    }
}

uint32_t NodeManager::CreateLink(uint32_t inputId, uint32_t outputId)
{
    m_Links[m_NextLinkId++] = std::make_pair(inputId, outputId);
    return m_NextLinkId - 1;
}

void NodeManager::RemoveLink(uint32_t linkId)
{
    if (!LinkExists(linkId))
    {
        std::cerr << "[NodeManager] Link " << linkId << " does not exist" << std::endl;
        return;
    }

    m_Links.erase(linkId);
}

bool NodeManager::NodeExists(uint32_t nodeId)
{
    return m_Nodes.find(nodeId) != m_Nodes.end();
}

bool NodeManager::IOExists(uint32_t ioId)
{
    return m_IOs.find(ioId) != m_IOs.end();
}

bool NodeManager::LinkExists(uint32_t linkId)
{
    return m_Links.find(linkId) != m_Links.end();
}
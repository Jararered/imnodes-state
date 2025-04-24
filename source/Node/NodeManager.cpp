#include "NodeManager.hpp"

#include <imnodes.h>
#include <imgui.h>

#include <memory>

NodeManager::NodeManager()
{
}

uint32_t NodeManager::CreateNode(const std::string &name)
{
    // Create Node
    NodePtr node = std::make_shared<Node>();
    node->ID = m_NextNodeId++;
    node->Name = name;

    // Add to map
    m_NodeMap[node->ID] = node;

    // Add to map
    return node->ID;
}

uint32_t NodeManager::AddInput(uint32_t nodeId, const std::string &name)
{
    NodeIO input = NodeIO(nodeId, m_NextIOId, name);
    m_NodeMap[nodeId]->Inputs.push_back(input);
    m_RegisteredIOs.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
}

uint32_t NodeManager::AddOutput(uint32_t nodeId, const std::string &name)
{
    NodeIO output = NodeIO(nodeId, m_NextIOId, name);
    m_NodeMap[nodeId]->Outputs.push_back(output);
    m_RegisteredIOs.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
}

void NodeManager::RemoveInput(uint32_t nodeId, uint32_t inputId)
{
    NodePtr node = m_NodeMap[nodeId];

    // Search through links for the input id
    for (auto &[linkId, inputOutput] : m_LinkMap)
    {
        if (inputOutput.first == inputId)
        {
            RemoveLink(linkId);
        }
    }
}

void NodeManager::RemoveOutput(uint32_t nodeId, uint32_t outputId)
{
    NodePtr node = m_NodeMap[nodeId];

    // Search through links for the output id
    for (auto &[linkId, inputOutput] : m_LinkMap)
    {
        if (inputOutput.second == outputId)
        {
            RemoveLink(linkId);
        }
    }
}

void NodeManager::RenameInput(uint32_t nodeId, uint32_t inputId, const std::string &name)
{
    NodePtr node = m_NodeMap[nodeId];
    for (auto &input : node->Inputs)
    {
        if (input.ID == inputId)
        {
            input.Name = name;
        }
    }
}

void NodeManager::RenameOutput(uint32_t nodeId, uint32_t outputId, const std::string &name)
{
    NodePtr node = m_NodeMap[nodeId];
    assert(node != nullptr);
    for (auto &output : node->Outputs)
    {
        if (output.ID == outputId)
        {
            output.Name = name;
        }
    }
}

uint32_t NodeManager::CreateLink(uint32_t inputId, uint32_t outputId)
{
    m_LinkMap[m_NextLinkId++] = std::make_pair(inputId, outputId);
    return m_NextLinkId - 1;
}

void NodeManager::RemoveNode(uint32_t nodeId)
{
    NodePtr node = m_NodeMap[nodeId];

    // Remove all inputs
    for (auto &input : node->Inputs)
    {
        m_RegisteredIOs.erase(input.ID);

        // Search through links to see if and contain the input id
        for (auto &[linkId, inputOutput] : m_LinkMap)
        {
            if (inputOutput.first == input.ID || inputOutput.second == input.ID)
            {
                RemoveLink(linkId);
            }
        }
    }

    // Remove all outputs
    for (auto &output : node->Outputs)
    {
        m_RegisteredIOs.erase(output.ID);

        // Search through links to see if and contain the output id
        for (auto &[linkId, inputOutput] : m_LinkMap)
        {
            if (inputOutput.first == output.ID || inputOutput.second == output.ID)
            {
                RemoveLink(linkId);
            }
        }
    }

    m_NodeMap.erase(nodeId);
    m_RegisteredNodes.erase(nodeId);
}

void NodeManager::RemoveLink(uint32_t linkId)
{
    m_RegisteredLinks.erase(linkId);
    m_LinkMap.erase(linkId);
}

#include "NodeManager.hpp"

#include <imnodes.h>
#include <imgui.h>

#include <memory>

NodeManager::NodeManager()
{
    uint32_t node1 = CreateNode("Node 1");
    uint32_t node1output1 = AddOutput(node1, "Output 1");

    uint32_t node2 = CreateNode("Node 2");
    uint32_t node2input1 = AddInput(node2, "Input 1");

    CreateLink(node1output1, node2input1);
}

uint32_t NodeManager::CreateNode(const std::string &name)
{
    // Create Node
    NodePtr node = std::make_shared<Node>();
    node->ID = m_NextNodeId++;
    node->Name = name;

    // Add to map
    m_NodeMap[node->ID] = node;

    // Add placeholder IOs
    // AddInput(node->ID, "Input 1");
    // AddOutput(node->ID, "Output 1");

    // Add to map
    return node->ID;
}

uint32_t NodeManager::AddInput(uint32_t nodeId, const std::string &name)
{
    NodeIO input = NodeIO(nodeId, m_NextIOId, name);
    m_NodeMap[nodeId]->Inputs.push_back(input);
    m_UsedIOIds.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
}

uint32_t NodeManager::AddOutput(uint32_t nodeId, const std::string &name)
{
    NodeIO output = NodeIO(nodeId, m_NextIOId, name);
    m_NodeMap[nodeId]->Outputs.push_back(output);
    m_UsedIOIds.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
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
        m_UsedIOIds.erase(input.ID);

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
        m_UsedIOIds.erase(output.ID);

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
    m_UsedNodeIds.erase(nodeId);
}

void NodeManager::RemoveLink(uint32_t linkId)
{
    m_UsedLinkIds.erase(linkId);
    m_LinkMap.erase(linkId);
}


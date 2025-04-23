#include "NodeManager.hpp"

#include <imnodes.h>
#include <imgui.h>

NodeManager::NodeManager()
{
    uint32_t node1 = CreateNode("Node 1");
    uint32_t node1input1 = AddInput(node1, "Input 1");
    uint32_t node1input2 = AddInput(node1, "Input 2");
    uint32_t node1output1 = AddOutput(node1, "Output 1");
    uint32_t node1output2 = AddOutput(node1, "Output 2");

    uint32_t node2 = CreateNode("Node 2");
    uint32_t node2input1 = AddInput(node2, "Input 1");
    uint32_t node2input2 = AddInput(node2, "Input 2");
    uint32_t node2output1 = AddOutput(node2, "Output 1");
    uint32_t node2output2 = AddOutput(node2, "Output 2");

    CreateLink(node1output1, node2input1);
    CreateLink(node1output2, node2input2);
}

uint32_t NodeManager::CreateNode(const std::string &name)
{
    Node node = {m_NextNodeId++, name, {}, {}};
    m_Nodes[node.m_Id] = node;
    return node.m_Id;
}

uint32_t NodeManager::AddInput(uint32_t nodeId, std::string name)
{
    NodeIO input = NodeIO(nodeId, m_NextIOId, name);
    m_Nodes[nodeId].m_Inputs.push_back(input);
    m_UsedIOIds.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
}

uint32_t NodeManager::AddOutput(uint32_t nodeId, std::string name)
{
    NodeIO output = NodeIO(nodeId, m_NextIOId, name);
    m_Nodes[nodeId].m_Outputs.push_back(output);
    m_UsedIOIds.insert(m_NextIOId);
    m_NextIOId++;
    return m_NextIOId - 1;
}

uint32_t NodeManager::CreateLink(uint32_t inputId, uint32_t outputId)
{
    m_Links[m_NextLinkId++] = std::make_pair(inputId, outputId);
    return m_NextLinkId - 1;
}

void NodeManager::RemoveNode(uint32_t nodeId)
{
    Node node = m_Nodes[nodeId];

    // Remove all inputs
    for (const auto &input : node.m_Inputs)
    {
        m_UsedIOIds.erase(input.m_Id);
    }

    // Remove all outputs
    for (const auto &output : node.m_Outputs)
    {
        m_UsedIOIds.erase(output.m_Id);
    }

    // Find all links that contain the node io ids and remove them
    for (const auto &[linkId, inputOutput] : m_Links)
    {
        if (inputOutput.first == nodeId || inputOutput.second == nodeId)
        {
            RemoveLink(linkId);
        }
    }

    m_Nodes.erase(nodeId);
    m_UsedNodeIds.erase(nodeId);
}

void NodeManager::RemoveLink(uint32_t linkId)
{
    m_UsedLinkIds.erase(linkId);
    m_Links.erase(linkId);
}

const void NodeManager::Render() const
{
    for (const auto &[id, node] : m_Nodes)
    {
        node.Render();
    }

    for (const auto &[linkId, inputOutput] : m_Links)
    {
        ImNodes::Link(linkId, inputOutput.first, inputOutput.second);
    }
}

const void NodeManager::RenderDetails() const
{
    ImGui::Begin("Node Manager");

    // Shows all ids
    ImGui::Text("Node IDs: %d", m_Nodes.size());
    ImGui::Text("IO IDs: %d", m_UsedIOIds.size());
    ImGui::Text("Link IDs: %d", m_Links.size());

    ImGui::Separator();
    ImGui::Text("Nodes");

    // Shows all node ids
    for (const auto &[name, node] : m_Nodes)
    {
        ImGui::Text("Node ID: %d", node.m_Id);
    }

    ImGui::Separator();
    ImGui::Text("IOs");

    // Shows all io ids
    for (const auto &ioId : m_UsedIOIds)
    {
        ImGui::Text("IO ID: %d", ioId);
    }

    ImGui::Separator();
    ImGui::Text("Links");

    // Shows all link ids
    for (const auto &[linkId, inputoutput] : m_Links)
    {
        ImGui::Text("Link ID: %d", linkId);
    }

    ImGui::End();
}
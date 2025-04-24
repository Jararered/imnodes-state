#include "NodeLayer.hpp"

#include <imnodes.h>
#include <imnodes_internal.h>
#include <imgui.h>

#include <iostream>

void NodeLayer::Update()
{
    LinkEvents();
    LayerEvents();
    NodeEvents();
}

void NodeLayer::Render()
{
    RenderNodeEditor();
    RenderDetails();
}

void NodeLayer::RenderNodeEditor()
{
    ImGui::Begin(m_Name.c_str());
    ImNodes::BeginNodeEditor();

    for (const auto &[id, node] : m_NodeManager.GetNodeMap())
    {
        ImNodes::BeginNode(node->ID);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node->Name.c_str());
        ImNodes::EndNodeTitleBar();

        // Dummy for unique identifier
        // This is required as ImGui does not use the titlebar as a unique identifier
        if (node->Inputs.size() == 0 && node->Outputs.size() == 0)
        {
            ImGui::Dummy(ImVec2(0, 0));
        }

        for (const NodeIO &input : node->Inputs)
        {
            ImNodes::BeginInputAttribute(input.ID);
            ImGui::TextUnformatted(input.Name.c_str());
            ImNodes::EndInputAttribute();
        }

        for (const NodeIO &output : node->Outputs)
        {
            ImNodes::BeginOutputAttribute(output.ID);
            ImGui::TextUnformatted(output.Name.c_str());
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }

    for (auto &[linkId, ioIds] : m_NodeManager.GetLinkMap())
    {
        ImNodes::Link(linkId, ioIds.first, ioIds.second);
    }

    ImNodes::EndNodeEditor();
    ImGui::End();
}

void NodeLayer::RenderDetails()
{
    ImGui::Begin("Node Manager");

    // Shows totals
    ImGui::Text("Total Nodes: %d", m_NodeManager.GetNodeMap().size());
    ImGui::Text("Total IOs: %d", m_NodeManager.GetIOs().size());
    ImGui::Text("Total Links: %d", m_NodeManager.GetLinkMap().size());

    ImGui::Separator();

    // Shows all node ids
    ImGui::Text("Nodes");
    for (auto &[id, node] : m_NodeManager.GetNodeMap())
    {
        ImGui::Text("Node ID: %d", node->ID);
    }

    ImGui::Separator();

    // Shows all io ids
    ImGui::Text("IOs");
    for (auto &ioId : m_NodeManager.GetIOs())
    {
        ImGui::Text("IO ID: %d", ioId);
    }

    ImGui::Separator();

    // Shows all link ids
    ImGui::Text("Links");
    for (auto &[linkId, inputoutput] : m_NodeManager.GetLinkMap())
    {
        ImGui::Text("Link ID: %d", linkId);
    }

    ImGui::End();
}

void NodeLayer::LinkEvents()
{
    int inputId, outputId;
    if (ImNodes::IsLinkCreated(&inputId, &outputId))
    {
        m_NodeManager.CreateLink(inputId, outputId);
    }

    // Handle link deletion if middle mouse button is clicked while hovering over a link
    int linkIdHovered;
    if (ImNodes::IsLinkHovered(&linkIdHovered))
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
        {
            m_NodeManager.RemoveLink(linkIdHovered);
        }
    }

    int nodeIdHovered;
    if (ImNodes::IsNodeHovered(&nodeIdHovered))
    {
        // Delete node if middle mouse button is clicked while hovering over a node
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
        {
            m_NodeManager.RemoveNode(nodeIdHovered);
        }
    }

    // Handle node deletion if delete key is pressed while more than one node is selected
    uint32_t numNodesSelected = ImNodes::NumSelectedNodes();
    int selectedNodeIds[numNodesSelected];
    ImNodes::GetSelectedNodes(selectedNodeIds);
    if (numNodesSelected > 0)
    {
        // Handle delete key press
        if (ImGui::IsKeyPressed(ImGuiKey_Delete))
        {
            for (uint32_t i = 0; i < numNodesSelected; i++)
            {
                m_NodeManager.RemoveNode(selectedNodeIds[i]);
            }
        }
    }

    // Handle link deletion if delete key is pressed while more than one link is selected
    uint32_t numLinksSelected = ImNodes::NumSelectedLinks();
    int selectedLinkIds[numLinksSelected];
    ImNodes::GetSelectedLinks(selectedLinkIds);
    if (numLinksSelected > 0)
    {
        // Handle delete key press
        if (ImGui::IsKeyPressed(ImGuiKey_Delete))
        {
            for (uint32_t i = 0; i < numLinksSelected; i++)
            {
                m_NodeManager.RemoveLink(selectedLinkIds[i]);
            }
        }
    }
}

void NodeLayer::LayerEvents()
{
    // Layer Right Click - Show Context Menu
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        // Make sure no nodes are hovered
        if (ImNodes::GetCurrentContext()->HoveredNodeIdx.HasValue() == false)
        {
            ImGui::OpenPopup("LayerContextMenu");
        }
    }

    // Show Context Menu
    if (ImGui::BeginPopupContextWindow("LayerContextMenu"))
    {
        if (ImGui::MenuItem("Add Node"))
        {
            m_NodeManager.CreateNode();
        }
        ImGui::EndPopup();
    }
}

void NodeLayer::NodeEvents()
{
    // Node Right Click - Show Context Menu
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        // Make sure only one node is selected
        if (ImNodes::NumSelectedNodes() == 1)
        {
            ImGui::OpenPopup("NodeContextMenu");
        }
    }

    // Show Context Menu
    if (ImGui::BeginPopupContextWindow("NodeContextMenu"))
    {
        int selectedNodeId;
        ImNodes::GetSelectedNodes(&selectedNodeId);

        bool hasInputs = m_NodeManager.GetNodeMap()[selectedNodeId]->Inputs.size() > 0;
        bool hasOutputs = m_NodeManager.GetNodeMap()[selectedNodeId]->Outputs.size() > 0;

        if (ImGui::MenuItem("Add Input"))
        {
            m_NodeManager.AddInput(selectedNodeId);
        }

        if (ImGui::MenuItem("Add Output"))
        {
            m_NodeManager.AddOutput(selectedNodeId);
        }

        // Add separator if there are inputs or outputs
        if (hasInputs || hasOutputs)
        {
            ImGui::Separator();
        }

        // Submenu for removing inputs and outputs
        if (hasInputs)
        {
            if (ImGui::BeginMenu("Inputs"))
            {
                // Loop over inputs to remove and add as menu items
                for (const auto &input : m_NodeManager.GetNodeMap()[selectedNodeId]->Inputs)
                {
                    if (ImGui::BeginMenu(input.Name.c_str()))
                    {
                        // TODO - Add input to rename
                        // if (ImGui::MenuItem("Rename"))
                        // {
                        //     m_NodeManager.RenameInput(selectedNodeId, input.ID, "New Name");
                        // }

                        if (ImGui::MenuItem("Remove"))
                        {
                            m_NodeManager.RemoveInput(selectedNodeId, input.ID);
                        }
                        ImGui::EndMenu();
                    }
                }
                ImGui::EndMenu();
            }
        }

        if (hasOutputs)
        {
            if (ImGui::BeginMenu("Outputs"))
            {
                // Loop over outputs to remove and add as menu items
                for (const auto &output : m_NodeManager.GetNodeMap()[selectedNodeId]->Outputs)
                {
                    if (ImGui::BeginMenu(output.Name.c_str()))
                    {
                        // TODO - Add output to rename
                        // if (ImGui::MenuItem("Rename"))
                        // {
                        //     m_NodeManager.RenameOutput(selectedNodeId, output.ID, "New Name");
                        // }

                        if (ImGui::MenuItem("Remove"))
                        {
                            m_NodeManager.RemoveOutput(selectedNodeId, output.ID);
                        }
                        ImGui::EndMenu();
                    }
                }
                ImGui::EndMenu();
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Delete"))
        {
            m_NodeManager.RemoveNode(selectedNodeId);
        }

        ImGui::EndPopup();
    }
}

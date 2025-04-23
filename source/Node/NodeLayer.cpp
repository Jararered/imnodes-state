#include "NodeLayer.hpp"

#include <imnodes.h>
#include <imgui.h>

void NodeLayer::Update()
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

        // Show context menu if right mouse button is clicked while hovering over a node
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("NodeContextMenu");
        }
        if (ImGui::BeginPopup("NodeContextMenu"))
        {
            ImGui::Text("Node Context Menu");
            ImGui::EndPopup();
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

    HandleNodeHover();
}

const void NodeLayer::Render() const
{
    ImGui::Begin("Node Editor");

    m_NodeManager.RenderDetails();

    ImNodes::BeginNodeEditor();
    m_NodeManager.Render();
    ImNodes::EndNodeEditor();

    ImGui::End();
}

void NodeLayer::HandleNodeHover()
{
    int nodeIdHovered;
    if (ImNodes::IsNodeHovered(&nodeIdHovered))
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("NodeContextMenu");
        }
    }

    if (ImGui::BeginPopup("NodeContextMenu"))
    {
        ImGui::Text("Context Menu");
        if (ImGui::MenuItem("Delete"))
        {
            m_NodeManager.RemoveNode(nodeIdHovered);
        }
        ImGui::EndPopup();
    }
}

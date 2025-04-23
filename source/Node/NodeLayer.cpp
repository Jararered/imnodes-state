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

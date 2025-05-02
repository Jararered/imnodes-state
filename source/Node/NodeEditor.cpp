#include "NodeEditor.hpp"

#include <imgui.h>
#include <imnodes.h>
#include <imnodes_internal.h>

#include <iostream>

void NodeEditor::Render()
{
    RenderNodeEditor();
    RenderMenuBar();

    // Update states after rendering Node Editor
    ProcessLinkEvents();

    UpdateSelectedStates();
    UpdateHoveredStates();
    UpdateClickedStates();

    RenderState();
    RenderToolTip();
    RenderContextMenu();
}

void NodeEditor::Update()
{
    m_NodeManager.Update();
}

void NodeEditor::ProcessLinkEvents()
{
    int inputId, outputId;
    if (ImNodes::IsLinkCreated(&inputId, &outputId))
    {
        m_NodeManager.CreateLink(inputId, outputId);
    }
}

void NodeEditor::UpdateSelectedStates()
{
    // This function can return a negative number if no nodes are selected
    int numSelectedNodes = ImNodes::NumSelectedNodes();
    if (numSelectedNodes < 0)
    {
        m_SelectedNodes.clear();
    }
    else
    {
        int selectedNodes[128] = {-1};
        ImNodes::GetSelectedNodes(selectedNodes);
        m_SelectedNodes.clear();
        for (int i = 0; i < numSelectedNodes; i++)
        {
            if (selectedNodes[i] > -1)
            {
                m_SelectedNodes.insert(selectedNodes[i]);
            }
        }
    }

    // This function can return a negative number if no links are selected
    int numSelectedLinks = ImNodes::NumSelectedLinks();
    if (numSelectedLinks < 0)
    {
        m_SelectedLinks.clear();
    }
    else
    {
        int selectedLinks[128] = {-1};
        ImNodes::GetSelectedLinks(selectedLinks);
        m_SelectedLinks.clear();
        for (int i = 0; i < numSelectedLinks; i++)
        {
            if (selectedLinks[i] > -1)
            {
                m_SelectedLinks.insert(selectedLinks[i]);
            }
        }
    }
}

void NodeEditor::UpdateHoveredStates()
{
    int hoveredNode = -1;
    ImNodes::IsNodeHovered(&hoveredNode);
    m_HoveredNodeID = hoveredNode;

    int hoveredLink = -1;
    ImNodes::IsLinkHovered(&hoveredLink);
    m_HoveredLinkID = hoveredLink;

    int hoveredPin = -1;
    ImNodes::IsPinHovered(&hoveredPin);
    m_HoveredPinID = hoveredPin;
}

void NodeEditor::UpdateClickedStates()
{
    bool isRightClick = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    if (isRightClick)
    {
        m_ClickedNodeID = m_HoveredNodeID;
        m_ClickedLinkID = m_HoveredLinkID;
        m_ClickedPinID = m_HoveredPinID;
    }
}

void NodeEditor::RenderNodeEditor()
{
    ImGui::Begin(m_Name.c_str());
    ImNodes::BeginNodeEditor();

    RenderNodes();
    RenderLinks();
    RenderMiniMap();

    ImNodes::EndNodeEditor();
    ImGui::End();
}

void NodeEditor::RenderNodes()
{
    for (const auto& nodeId : m_NodeManager.GetRegisteredNodes())
    {
        const auto& nodePos = m_NodeManager.GetNodeData(nodeId).Position;
        ImVec2 nodePosVec = ImVec2(nodePos.X, nodePos.Y);
        // ImNodes::SetNodeScreenSpacePos(nodeId, nodePosVec);

        ImNodes::BeginNode(nodeId);

        ColorData colors = m_NodeManager.GetNodeData(nodeId).Colors;

        ImNodes::PushColorStyle(ImNodesCol_TitleBar, colors.NormalColor);
        ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, colors.HoverColor);
        ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, colors.SelectedColor);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(m_NodeManager.GetNodeData(nodeId).Name.c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        const auto& outputIDs = m_NodeManager.GetNodeData(nodeId).OutputIDs;
        const auto& inputIDs = m_NodeManager.GetNodeData(nodeId).InputIDs;

        // This is required as ImGui does not use the titlebar as a unique identifier
        if (inputIDs.size() == 0 && outputIDs.size() == 0)
        {
            ImGui::Dummy(ImVec2(0, 0));

            // End node early to avoid drawing pins
            ImNodes::EndNode();
            continue;
        }

        for (const auto& inputID : inputIDs)
        {
            ColorData colors = m_NodeManager.GetPinData(inputID).Colors;

            ImNodes::PushColorStyle(ImNodesCol_Pin, colors.NormalColor);
            ImNodes::PushColorStyle(ImNodesCol_PinHovered, colors.HoverColor);

            ImNodes::BeginInputAttribute(inputID);
            ImGui::TextUnformatted(m_NodeManager.GetPinData(inputID).Name.c_str());
            ImNodes::EndInputAttribute();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }

        for (const auto& outputID : outputIDs)
        {
            ColorData colors = m_NodeManager.GetPinData(outputID).Colors;

            ImNodes::PushColorStyle(ImNodesCol_Pin, colors.NormalColor);
            ImNodes::PushColorStyle(ImNodesCol_PinHovered, colors.HoverColor);

            ImNodes::BeginOutputAttribute(outputID);
            ImGui::TextUnformatted(m_NodeManager.GetPinData(outputID).Name.c_str());
            ImNodes::EndOutputAttribute();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }

        ImNodes::EndNode();
    }
}

void NodeEditor::RenderLinks()
{
    for (const auto& linkId : m_NodeManager.GetRegisteredLinks())
    {
        const auto& linkData = m_NodeManager.GetLinkData(linkId);
        ColorData colors = linkData.Colors;

        ImNodes::PushColorStyle(ImNodesCol_Link, colors.NormalColor);
        ImNodes::PushColorStyle(ImNodesCol_LinkHovered, colors.HoverColor);
        ImNodes::PushColorStyle(ImNodesCol_LinkSelected, colors.SelectedColor);

        ImNodes::Link(linkId, linkData.Pin1ID, linkData.Pin2ID);

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
    }
}

void NodeEditor::RenderMiniMap()
{
    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);
}

void NodeEditor::RenderState()
{
    ImGui::Begin("Node Editor State");

    // Selected Nodes
    std::string selectedNodesString = "Selected Node IDs: ";
    auto it = m_SelectedNodes.begin();
    auto end = m_SelectedNodes.end();
    while (it != end)
    {
        selectedNodesString += std::to_string(*it);
        it++;
        if (it != end)
        {
            selectedNodesString += ", ";
        }
    }

    // Selected Links
    std::string selectedLinksString = "Selected Link IDs: ";
    it = m_SelectedLinks.begin();
    end = m_SelectedLinks.end();
    while (it != end)
    {
        selectedLinksString += std::to_string(*it);
        it++;
        if (it != end)
        {
            selectedLinksString += ", ";
        }
    }

    ImGui::Text(selectedNodesString.c_str());
    ImGui::Text(selectedLinksString.c_str());

    ImGui::Separator();

    // Hovered Node
    std::string hoveredNodeString = "Hovered Node ID: ";
    if (m_HoveredNodeID != -1)
    {
        hoveredNodeString += std::to_string(m_HoveredNodeID);
    }

    // Hovered Link
    std::string hoveredLinkString = "Hovered Link ID: ";
    if (m_HoveredLinkID != -1)
    {
        hoveredLinkString += std::to_string(m_HoveredLinkID);
    }

    // Hovered Pin
    std::string hoveredPinString = "Hovered Pin ID: ";
    if (m_HoveredPinID != -1)
    {
        hoveredPinString += std::to_string(m_HoveredPinID);
    }

    ImGui::Text(hoveredNodeString.c_str());
    ImGui::Text(hoveredLinkString.c_str());
    ImGui::Text(hoveredPinString.c_str());

    ImGui::Separator();

    ImGui::Text("Clicked Node ID: %d", m_ClickedNodeID);
    ImGui::Text("Clicked Link ID: %d", m_ClickedLinkID);
    ImGui::Text("Clicked Pin ID: %d", m_ClickedPinID);

    ImGui::End();
}

void NodeEditor::RenderToolTip()
{
    // Hovered pin takes pri
    if (IsPinHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Pin ID: %d", m_HoveredPinID);
        ImGui::EndTooltip();
    }
    else if (IsLinkHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Link ID: %d", m_HoveredLinkID);
        ImGui::EndTooltip();
    }
    else if (IsNodeHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Node ID: %d", m_HoveredNodeID);
        ImGui::EndTooltip();
    }
}

void NodeEditor::RenderContextMenu()
{
    bool isRightClick = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    bool isPinHovered = IsPinHovered();
    bool isLinkHovered = IsLinkHovered();
    bool isNodeHovered = IsNodeHovered();

    if (isRightClick && !isPinHovered && !isLinkHovered && !isNodeHovered)
    {
        ImGui::OpenPopup("EditorContextMenu");
    }

    if (isRightClick && isPinHovered)
    {
        ImGui::OpenPopup("PinContextMenu");
    }
    else if (isRightClick && isLinkHovered)
    {
        ImGui::OpenPopup("LinkContextMenu");
    }
    else if (isRightClick && isNodeHovered)
    {
        ImGui::OpenPopup("NodeContextMenu");
    }

    if (ImGui::BeginPopupContextWindow("EditorContextMenu"))
    {
        if (ImGui::MenuItem("Add Node"))
        {
            ImVec2 cursorPos = ImGui::GetMousePos();
            m_NodeManager.CreateNode(cursorPos.x, cursorPos.y);
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupContextWindow("PinContextMenu"))
    {
        if (ImGui::BeginMenu("Set Color"))
        {
            for (const auto& [colorName, colorData] : ColorDataPresets)
            {
                if (ImGui::MenuItem(colorName.c_str()))
                {
                    m_NodeManager.SetPinColor(m_ClickedPinID, colorData);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupContextWindow("LinkContextMenu"))
    {
        if (ImGui::BeginMenu("Set Color"))
        {
            for (const auto& [colorName, colorData] : ColorDataPresets)
            {
                if (ImGui::MenuItem(colorName.c_str()))
                {
                    m_NodeManager.SetLinkColor(m_ClickedLinkID, colorData);
                }
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Delete"))
        {
            m_NodeManager.RemoveLink(m_ClickedLinkID);
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupContextWindow("NodeContextMenu"))
    {
        int clickedNodeId = m_ClickedNodeID;
        if (ImGui::BeginMenu("Pins"))
        {
            if (ImGui::MenuItem("Add Input"))
            {
                m_NodeManager.CreatePin(clickedNodeId, PinType::Input);
            }

            if (ImGui::MenuItem("Add Output"))
            {
                m_NodeManager.CreatePin(clickedNodeId, PinType::Output);
            }
            ImGui::EndMenu();
        }

        bool hasInputs = m_NodeManager.GetNodeData(clickedNodeId).InputIDs.size() > 0;
        if (hasInputs)
        {
            if (ImGui::BeginMenu("Inputs"))
            {
                for (const auto& inputID : m_NodeManager.GetNodeData(clickedNodeId).InputIDs)
                {
                    if (ImGui::MenuItem(m_NodeManager.GetPinData(inputID).Name.c_str()))
                    {
                        m_NodeManager.RemovePin(clickedNodeId, inputID);
                    }
                }

                ImGui::EndMenu();
            }
        }

        bool hasOutputs = m_NodeManager.GetNodeData(clickedNodeId).OutputIDs.size() > 0;
        if (hasOutputs)
        {
            if (ImGui::BeginMenu("Outputs"))
            {
                for (const auto& outputID : m_NodeManager.GetNodeData(clickedNodeId).OutputIDs)
                {
                    if (ImGui::MenuItem(m_NodeManager.GetPinData(outputID).Name.c_str()))
                    {
                        m_NodeManager.RemovePin(clickedNodeId, outputID);
                    }
                }

                ImGui::EndMenu();
            }
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Set Color"))
        {
            for (const auto& [colorName, colorData] : ColorDataPresets)
            {
                if (ImGui::MenuItem(colorName.c_str()))
                {
                    m_NodeManager.SetNodeColor(clickedNodeId, colorData);
                }
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Delete"))
        {
            m_NodeManager.RemoveNode(clickedNodeId);
        }

        ImGui::EndPopup();
    }
}

void NodeEditor::RenderMenuBar()
{
}

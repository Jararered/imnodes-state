#include "NodeEditor.hpp"

#include <imgui.h>
#include <imnodes.h>
#include <imnodes_internal.h>

#include <iostream>

void NodeEditor::Update()
{
    m_NodeManager.Update();

    UpdateSelectedState();
    UpdateHoveredState();
}

void NodeEditor::UpdateSelectedState()
{
    UpdateSelectedNodes();
    UpdateSelectedLinks();
}

void NodeEditor::UpdateSelectedNodes()
{
    int numSelectedNodes = ImNodes::NumSelectedNodes();
    if (numSelectedNodes < 0)
    {
        m_SelectedNodes.clear();
        return;
    }

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

void NodeEditor::UpdateSelectedLinks()
{
    int numSelectedLinks = ImNodes::NumSelectedLinks();
    if (numSelectedLinks < 0)
    {
        m_SelectedLinks.clear();
        return;
    }

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

void NodeEditor::UpdateHoveredState()
{
    UpdateHoveredNode();
    UpdateHoveredLink();
    UpdateHoveredPin();
}

void NodeEditor::UpdateHoveredNode()
{
    int hoveredNode = -1;
    ImNodes::IsNodeHovered(&hoveredNode);
    m_HoveredNodeID = hoveredNode;
}

void NodeEditor::UpdateHoveredLink()
{
    int hoveredLink = -1;
    ImNodes::IsLinkHovered(&hoveredLink);
    m_HoveredLinkID = hoveredLink;
}

void NodeEditor::UpdateHoveredPin()
{
    int hoveredPin = -1;
    ImNodes::IsPinHovered(&hoveredPin);
    m_HoveredPinID = hoveredPin;
}

void NodeEditor::Render()
{
    m_NodeManager.Render();

    RenderNodeEditor();
    RenderState();
    RenderToolTip();

    ProcessLinkEvents();
    ProcessLayerEvents();
    ProcessNodeEvents();
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
        ImVec2 nodePosVec = ImVec2(nodePos.first, nodePos.second);
        ImNodes::SetNodeScreenSpacePos(nodeId, nodePosVec);

        ImNodes::BeginNode(nodeId);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(m_NodeManager.GetNodeData(nodeId).Name.c_str());
        ImNodes::EndNodeTitleBar();

        const auto& inputIDs = m_NodeManager.GetNodeData(nodeId).InputIDs;
        const auto& outputIDs = m_NodeManager.GetNodeData(nodeId).OutputIDs;

        // This is required as ImGui does not use the titlebar as a unique identifier
        if (inputIDs.size() == 0 && outputIDs.size() == 0)
        {
            ImGui::Dummy(ImVec2(0, 0));
        }

        for (const auto& inputID : inputIDs)
        {
            ImNodes::BeginInputAttribute(inputID);
            ImGui::TextUnformatted(m_NodeManager.GetPinData(inputID).Name.c_str());
            ImNodes::EndInputAttribute();
        }

        for (const auto& outputID : outputIDs)
        {
            ImNodes::BeginOutputAttribute(outputID);
            ImGui::TextUnformatted(m_NodeManager.GetPinData(outputID).Name.c_str());
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }
}

void NodeEditor::RenderLinks()
{

    for (const auto& linkId : m_NodeManager.GetRegisteredLinks())
    {
        const auto& linkData = m_NodeManager.GetLinkData(linkId);
        ImNodes::Link(linkId, linkData.Pin1ID, linkData.Pin2ID);
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

    ImGui::End();
}

void NodeEditor::RenderToolTip()
{
    if (m_HoveredPinID != -1)
    {
        ImGui::BeginTooltip();
        ImGui::Text("Pin ID: %d", m_HoveredPinID);
        ImGui::EndTooltip();
    }
    else if (m_HoveredNodeID != -1)
    {
        ImGui::BeginTooltip();
        ImGui::Text("Node ID: %d", m_HoveredNodeID);
        ImGui::EndTooltip();
    }
    else if (m_HoveredLinkID != -1)
    {
        ImGui::BeginTooltip();
        ImGui::Text("Link ID: %d", m_HoveredLinkID);
        ImGui::EndTooltip();
    }
}

void NodeEditor::ProcessLinkEvents()
{
    int inputId, outputId;
    if (ImNodes::IsLinkCreated(&inputId, &outputId))
    {
        m_NodeManager.CreateLink(inputId, outputId);
    }

    bool isMouseRightClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    if (isMouseRightClicked)
    {
        int numSelectedLinks = m_SelectedLinks.size();
        int numSelectedNodes = m_SelectedNodes.size();
        bool isLinkHovered = IsLinkHovered();
        if (numSelectedLinks > 0 && numSelectedNodes == 0 && isLinkHovered)
        {
            if (numSelectedLinks == 1)
            {
                ImGui::OpenPopup("SingleLinkContextMenu");
            }

            if (numSelectedLinks > 1)
            {
                ImGui::OpenPopup("MultiLinkContextMenu");
            }
        }
    }

    if (ImGui::BeginPopup("SingleLinkContextMenu"))
    {
        if (ImGui::MenuItem("Delete"))
        {
            m_NodeManager.RemoveLinks(m_SelectedLinks);
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("MultiLinkContextMenu"))
    {
        std::string itemName = "Delete " + std::to_string(m_SelectedLinks.size()) + " links";
        if (ImGui::MenuItem(itemName.c_str()))
        {
            m_NodeManager.RemoveLinks(m_SelectedLinks);
        }

        ImGui::EndPopup();
    }
}

void NodeEditor::ProcessLayerEvents()
{
    // Layer Right Click - Show Context Menu
    bool isNothingHovered = m_HoveredNodeID == -1 && m_HoveredLinkID == -1 && m_HoveredPinID == -1;
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && isNothingHovered)
    {
        // Make sure no nodes are hovered
        if (m_HoveredNodeID == -1)
        {
            ImGui::OpenPopup("LayerContextMenu");
        }
    }

    // Show Context Menu
    if (ImGui::BeginPopupContextWindow("LayerContextMenu"))
    {
        if (ImGui::MenuItem("Add Node"))
        {
            ImVec2 cursorPos = ImGui::GetMousePos();
            m_NodeManager.CreateNode(cursorPos.x, cursorPos.y);
        }
        ImGui::EndPopup();
    }
}

void NodeEditor::ProcessNodeEvents()
{
    int numSelectedNodes = m_SelectedNodes.size();

    // Node Right Click - Show Context Menu
    bool isRightClick = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    bool isNodeHovered = IsNodeHovered();
    if (isRightClick && isNodeHovered)
    {
        // Make sure only one node is selected
        if (numSelectedNodes == 1)
        {
            int selectedNodeId = *m_SelectedNodes.begin();
            int hoveredNodeId = m_HoveredNodeID;
            if (selectedNodeId == hoveredNodeId)
            {
                ImGui::OpenPopup("SingleNodeContextMenu");
            }
        }

        if (numSelectedNodes > 1)
        {
            ImGui::OpenPopup("MultiNodeContextMenu");
        }
    }

    if (ImGui::BeginPopupContextWindow("SingleNodeContextMenu"))
    {
        int selectedNodeId = *m_SelectedNodes.begin();
        if (ImGui::BeginMenu("Pins"))
        {
            if (ImGui::MenuItem("Add Input"))
            {
                m_NodeManager.CreatePin(selectedNodeId, PinType::Input);
            }

            if (ImGui::MenuItem("Add Output"))
            {
                m_NodeManager.CreatePin(selectedNodeId, PinType::Output);
            }
            ImGui::EndMenu();
        }

        bool hasInputs = m_NodeManager.GetNodeData(selectedNodeId).InputIDs.size() > 0;
        if (hasInputs)
        {
            if (ImGui::BeginMenu("Inputs"))
            {
                for (const auto& inputID : m_NodeManager.GetNodeData(selectedNodeId).InputIDs)
                {
                    if (ImGui::MenuItem(m_NodeManager.GetPinData(inputID).Name.c_str()))
                    {
                        m_NodeManager.RemovePin(selectedNodeId, inputID);
                    }
                }

                ImGui::EndMenu();
            }
        }

        bool hasOutputs = m_NodeManager.GetNodeData(selectedNodeId).OutputIDs.size() > 0;
        if (hasOutputs)
        {
            if (ImGui::BeginMenu("Outputs"))
            {
                for (const auto& outputID : m_NodeManager.GetNodeData(selectedNodeId).OutputIDs)
                {
                    if (ImGui::MenuItem(m_NodeManager.GetPinData(outputID).Name.c_str()))
                    {
                        m_NodeManager.RemovePin(selectedNodeId, outputID);
                    }
                }

                ImGui::EndMenu();
            }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Delete"))
        {
            m_NodeManager.RemoveNodes(m_SelectedNodes);
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupContextWindow("MultiNodeContextMenu"))
    {
        std::string itemName = "Delete " + std::to_string(m_SelectedNodes.size()) + " nodes";
        if (ImGui::MenuItem(itemName.c_str()))
        {
            m_NodeManager.RemoveNodes(m_SelectedNodes);
        }

        ImGui::EndPopup();
    }
}

bool NodeEditor::IsNodeHovered()
{
    return m_HoveredNodeID != -1;
}

bool NodeEditor::IsLinkHovered()
{
    return m_HoveredLinkID != -1;
}

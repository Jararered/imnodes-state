#include "NodeLayer.hpp"

#include <imgui.h>
#include <imnodes.h>
#include <imnodes_internal.h>

#include <iostream>

void NodeLayer::Update()
{
    m_NodeManager.Update();

    UpdateSelectedState();
    UpdateHoveredState();
}

void NodeLayer::UpdateSelectedState()
{
    UpdateSelectedNodes();
    UpdateSelectedLinks();
}

void NodeLayer::UpdateSelectedNodes()
{
    int numSelectedNodes = ImNodes::NumSelectedNodes();
    if (numSelectedNodes < 0)
    {
        m_SelectedNodes.clear();
        return;
    }

    int selectedNodes[numSelectedNodes] = {-1};
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

void NodeLayer::UpdateSelectedLinks()
{
    int numSelectedLinks = ImNodes::NumSelectedLinks();
    if (numSelectedLinks < 0)
    {
        m_SelectedLinks.clear();
        return;
    }

    int selectedLinks[numSelectedLinks] = {-1};
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

void NodeLayer::UpdateHoveredState()
{
    UpdateHoveredNode();
    UpdateHoveredLink();
    UpdateHoveredPin();
}

void NodeLayer::UpdateHoveredNode()
{
    int hoveredNode = -1;
    ImNodes::IsNodeHovered(&hoveredNode);
    m_HoveredNodeID = hoveredNode;
}

void NodeLayer::UpdateHoveredLink()
{
    int hoveredLink = -1;
    ImNodes::IsLinkHovered(&hoveredLink);
    m_HoveredLinkID = hoveredLink;
}

void NodeLayer::UpdateHoveredPin()
{
    int hoveredPin = -1;
    ImNodes::IsPinHovered(&hoveredPin);
    m_HoveredPinID = hoveredPin;
}

void NodeLayer::Render()
{
    m_NodeManager.Render();

    RenderState();

    RenderNodeEditor();

    ProcessLinkEvents();
    ProcessLayerEvents();
    ProcessNodeEvents();
}

void NodeLayer::RenderNodeEditor()
{
    ImGui::Begin(m_Name.c_str());
    ImNodes::BeginNodeEditor();

    for (const auto &nodeId : m_NodeManager.GetRegisteredNodes())
    {
        ImNodes::BeginNode(nodeId);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(m_NodeManager.GetNodeData(nodeId).Name.c_str());
        ImNodes::EndNodeTitleBar();

        const auto &inputIDs = m_NodeManager.GetNodeData(nodeId).InputIDs;
        const auto &outputIDs = m_NodeManager.GetNodeData(nodeId).OutputIDs;

        // This is required as ImGui does not use the titlebar as a unique identifier
        if (inputIDs.size() == 0 && outputIDs.size() == 0)
        {
            ImGui::Dummy(ImVec2(0, 0));
        }

        for (const auto &inputID : inputIDs)
        {
            ImNodes::BeginInputAttribute(inputID);
            ImGui::TextUnformatted(m_NodeManager.GetPinData(inputID).Name.c_str());
            ImNodes::EndInputAttribute();
        }

        for (const auto &outputID : outputIDs)
        {
            ImNodes::BeginOutputAttribute(outputID);
            ImGui::TextUnformatted(m_NodeManager.GetPinData(outputID).Name.c_str());
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }

    for (const auto &linkId : m_NodeManager.GetRegisteredLinks())
    {
        const auto &linkData = m_NodeManager.GetLinkData(linkId);
        ImNodes::Link(linkId, linkData.Pin1ID, linkData.Pin2ID);
    }

    ImNodes::EndNodeEditor();
    ImGui::End();
}

void NodeLayer::ProcessLinkEvents()
{
    int inputId, outputId;
    if (ImNodes::IsLinkCreated(&inputId, &outputId))
    {
        m_NodeManager.CreateLink(inputId, outputId);
    }
}

void NodeLayer::ProcessLayerEvents()
{
    // Layer Right Click - Show Context Menu
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
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
            m_NodeManager.CreateNode();
        }
        ImGui::EndPopup();
    }
}

void NodeLayer::ProcessNodeEvents()
{
    int numSelectedNodes = m_SelectedNodes.size();

    // Node Right Click - Show Context Menu
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        // Make sure only one node is selected
        if (numSelectedNodes == 1)
        {
            ImGui::OpenPopup("SingleNodeContextMenu");
        }

        if (numSelectedNodes > 1)
        {
            ImGui::OpenPopup("MultiNodeContextMenu");
        }
    }

    // Show Context Menu
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

        // Submenu for removing inputs and outputs
        if (m_NodeManager.GetNodeData(selectedNodeId).InputIDs.size() > 0)
        {
            if (ImGui::BeginMenu("Inputs"))
            {
                for (const auto &inputID : m_NodeManager.GetNodeData(selectedNodeId).InputIDs)
                {
                    if (ImGui::MenuItem(m_NodeManager.GetPinData(inputID).Name.c_str()))
                    {
                        m_NodeManager.RemovePin(selectedNodeId, inputID);
                    }
                }

                ImGui::EndMenu();
            }
        }

        if (m_NodeManager.GetNodeData(selectedNodeId).OutputIDs.size() > 0)
        {
            if (ImGui::BeginMenu("Outputs"))
            {
                for (const auto &outputID : m_NodeManager.GetNodeData(selectedNodeId).OutputIDs)
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

void NodeLayer::RenderState()
{
    ImGui::Begin("Node State");

    // Selected Nodes
    std::string selectedNodesString = "Selected Node IDs: ";
    auto it = m_SelectedNodes.begin();
    auto end = m_SelectedNodes.end();
    while (it != end)
    {
        selectedNodesString += std::to_string(*it);
        ++it;
        if (it != end)
        {
            selectedNodesString += ", ";
        }
    }
    ImGui::Text(selectedNodesString.c_str());

    // Selected Links
    std::string selectedLinksString = "Selected Link IDs: ";
    it = m_SelectedLinks.begin();
    end = m_SelectedLinks.end();
    while (it != end)
    {
        selectedLinksString += std::to_string(*it);
        ++it;
        if (it != end)
        {
            selectedLinksString += ", ";
        }
    }
    ImGui::Text(selectedLinksString.c_str());

    // Selected Pins
    std::string selectedPinsString = "Selected Pin IDs: ";
    it = m_SelectedPins.begin();
    end = m_SelectedPins.end();
    while (it != end)
    {
        selectedPinsString += std::to_string(*it);
        ++it;
        if (it != end)
        {
            selectedPinsString += ", ";
        }
    }
    ImGui::Text(selectedPinsString.c_str());

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

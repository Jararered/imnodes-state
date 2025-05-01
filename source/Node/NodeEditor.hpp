#pragma once

#include <Node/NodeManager.hpp>
#include <Window/Layer.hpp>

class NodeEditor : public Layer
{
public:
    NodeEditor() : Layer("New Node Editor") {}
    NodeEditor(const std::string& name) : Layer(name) {}
    ~NodeEditor() override {}

    void Update() override;
    void Render() override;

    void RenderNodeEditor();
    void RenderNodes();
    void RenderLinks();
    void RenderMiniMap();
    void RenderState();
    void RenderToolTip();

    void UpdateSelectedState();
    void UpdateSelectedNodes();
    void UpdateSelectedLinks();

    void UpdateHoveredState();
    void UpdateHoveredNode();
    void UpdateHoveredLink();
    void UpdateHoveredPin();

    void ProcessLinkEvents();
    void ProcessLayerEvents();
    void ProcessNodeEvents();

    bool IsNodeHovered();
    bool IsLinkHovered();
    bool IsPinHovered();

private:
    NodeManager m_NodeManager;

    IDSet m_SelectedNodes;
    IDSet m_SelectedLinks;

    std::int32_t m_HoveredNodeID;
    std::int32_t m_HoveredLinkID;
    std::int32_t m_HoveredPinID;

    std::int32_t m_ClickedNodeID;
    std::int32_t m_ClickedLinkID;
    std::int32_t m_ClickedPinID;
};

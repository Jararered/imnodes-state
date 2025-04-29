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

private:
    NodeManager m_NodeManager;

    NodeIDSet m_SelectedNodes;
    LinkIDSet m_SelectedLinks;

    uint32_t m_HoveredNodeID;
    uint32_t m_HoveredLinkID;
    uint32_t m_HoveredPinID;
};

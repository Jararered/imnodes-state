#pragma once

#include <Window/Layer.hpp>
#include <Node/NodeManager.hpp>

#include <unordered_set>

class NodeEditor : public Layer
{
public:
    NodeEditor() : Layer("New Node Editor") {}
    ~NodeEditor() override {}

    void Update() override;
    void Render() override;

    void RenderNodeEditor();

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

    void RenderState();

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

#pragma once

#include <Window/Layer.hpp>
#include <Node/NodeManager.hpp>

#include <unordered_set>

class NodeLayer : public Layer
{
public:
    NodeLayer() : Layer("New Node Editor") {}
    ~NodeLayer() override {}

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

private:
    NodeManager m_NodeManager;

    NodeIDSet m_SelectedNodes;
    LinkIDSet m_SelectedLinks;

    uint32_t m_HoveredNodeID;
    uint32_t m_HoveredLinkID;
    uint32_t m_HoveredPinID;
};

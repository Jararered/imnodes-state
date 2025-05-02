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

private:
    void RenderNodeEditor();
    void RenderNodes();
    void RenderLinks();
    void RenderMiniMap();
    void RenderState();
    void RenderToolTip();
    void RenderContextMenu();
    void RenderMenuBar();

    void UpdateSelectedStates();
    void UpdateHoveredStates();
    void UpdateClickedStates();

    void ProcessLinkEvents();

    inline bool IsNodeHovered() const { return m_HoveredNodeID != -1; }
    inline bool IsLinkHovered() const { return m_HoveredLinkID != -1; }
    inline bool IsPinHovered() const { return m_HoveredPinID != -1; }

    inline size_t NumSelectedNodes() const { return m_SelectedNodes.size(); }
    inline size_t NumSelectedLinks() const { return m_SelectedLinks.size(); }

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

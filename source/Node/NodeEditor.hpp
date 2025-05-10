#pragma once

#include <Node/NodeManager.hpp>
#include <Window/Layer.hpp>

#include <optional>

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
    void UpdateClickedStates();

    void ProcessLinkEvents();

    std::optional<std::uint32_t> GetHoveredNode() const;
    std::optional<std::uint32_t> GetHoveredLink() const;
    std::optional<std::uint32_t> GetHoveredPin() const;

    inline size_t NumSelectedNodes() const { return m_SelectedNodes.size(); }
    inline size_t NumSelectedLinks() const { return m_SelectedLinks.size(); }

private:
    NodeManager m_NodeManager;

    IDSet m_SelectedNodes;
    IDSet m_SelectedLinks;

    std::optional<std::uint32_t> m_ClickedNodeID;
    std::optional<std::uint32_t> m_ClickedLinkID;
    std::optional<std::uint32_t> m_ClickedPinID;
};

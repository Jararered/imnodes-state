#pragma once

#include <Window/Layer.hpp>
#include <Node/NodeManager.hpp>

class NodeLayer : public Layer
{
public:
    NodeLayer() : Layer("New Node Editor") {}

    void Update() override;
    void Render() override;

    void RenderNodeEditor();
    void RenderDetails();

    void LinkEvents();
    void LayerEvents();
    void NodeEvents();

private:
    NodeManager m_NodeManager;
};

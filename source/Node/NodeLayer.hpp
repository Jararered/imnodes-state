#pragma once

#include <Window/Layer.hpp>
#include <Node/NodeManager.hpp>

class NodeLayer : public Layer
{
public:
    NodeLayer() = default;

    void Update() override;
    const void Render() const override;

    void HandleNodeHover();

private:
    NodeManager m_NodeManager;
};

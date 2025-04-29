#pragma once

#include "ObjectNode.hpp"
#include "ObjectPin.hpp"
#include "ObjectLink.hpp"

#include <Node/NodeManager.hpp>

#include <memory>
#include <vector>

class Simulation : public NodeManager
{
public:
    Simulation() = default;
    ~Simulation() = default;

    void Initialize();
    void Update();
    void Finalize();

    void AddObject(std::unique_ptr<ObjectNode> node);
    void AddObject(std::unique_ptr<ObjectPin> pin);
    void AddObject(std::unique_ptr<ObjectLink> link);

private:
    std::vector<std::unique_ptr<ObjectNode>> m_ObjectNodes;
    std::vector<std::unique_ptr<ObjectPin>> m_ObjectPins;
    std::vector<std::unique_ptr<ObjectLink>> m_ObjectLinks;
};
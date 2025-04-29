#pragma once

#include <Node/NodeTypes.hpp>

#include <cstdint>

struct ObjectNodeParameters
{
    uint32_t InitializeOrder = 0;
    uint32_t UpdateOrder = 0;
    uint32_t UpdateRate = 0;
};

class ObjectNode : public NodeData
{
public:
    ObjectNode() = default;
    ObjectNode(const ObjectNodeParameters& parameters);
    ~ObjectNode() = default;

    virtual void Initialize();
    virtual void Update();
    virtual void Finalize();

    virtual void ShowContextMenu();

private:
    ObjectNodeParameters m_Parameters;
};
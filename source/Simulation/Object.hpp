#pragma once

#include <Node/NodeTypes.hpp>

#include <cstdint>

struct ObjectParameters : NodeData
{
    uint32_t InitializeOrder = 0;
    uint32_t UpdateOrder = 0;
    uint32_t UpdateRate = 0;
};

class Object
{
public:
    Object() = default;
    Object(const ObjectParameters& parameters);
    ~Object() = default;

    virtual void Initialize();
    virtual void Update();
    virtual void Finalize();

private:
    ObjectParameters m_Parameters;
};
#pragma once

#include <Node/NodeManager.hpp>
#include <Node/NodeTypes.hpp>

#include <memory>
#include <vector>

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
    Object(const ObjectParameters& parameters) : m_Parameters(parameters)
    {
    }
    ~Object() = default;

    virtual void Initialize() {};
    virtual void Update() {};
    virtual void Finalize() {};

private:
    ObjectParameters m_Parameters;
};

class Simulation : public NodeManager
{
public:
    Simulation() = default;
    ~Simulation() = default;

    void Initialize()
    {
        for (auto& object : m_Objects)
        {
            object->Initialize();
        }
    }

    void Update()
    {
        for (auto& object : m_Objects)
        {
            object->Update();
        }
    }

    void Finalize()
    {
        for (auto& object : m_Objects)
        {
            object->Finalize();
        }
    }

    void AddObject(std::unique_ptr<Object> object)
    {
        m_Objects.push_back(std::move(object));
    }

private:
    std::vector<std::unique_ptr<Object>> m_Objects;
};
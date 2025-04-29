#pragma once

#include "Object.hpp"

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

    void AddObject(std::unique_ptr<Object> object);

private:
    std::vector<std::unique_ptr<Object>> m_Objects;
};
#include "Simulation.hpp"

void Simulation::Initialize()
{
    for (auto& object : m_ObjectNodes)
    {
        object->Initialize();
    }
}

void Simulation::Update()
{
    for (auto& object : m_ObjectNodes)
    {
        object->Update();
    }
}

void Simulation::Finalize()
{
    for (auto& object : m_ObjectNodes)
    {
        object->Finalize();
    }
}

void Simulation::AddObject(std::unique_ptr<ObjectNode> node)
{
    m_ObjectNodes.push_back(std::move(node));
}

void Simulation::AddObject(std::unique_ptr<ObjectPin> pin)
{
    m_ObjectPins.push_back(std::move(pin));
}

void Simulation::AddObject(std::unique_ptr<ObjectLink> link)
{
    m_ObjectLinks.push_back(std::move(link));
}

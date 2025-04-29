#include "Simulation.hpp"

void Simulation::Initialize()
{
    for (auto& object : m_Objects)
    {
        object->Initialize();
    }
}

void Simulation::Update()
{
    for (auto& object : m_Objects)
    {
        object->Update();
    }
}

void Simulation::Finalize()
{
    for (auto& object : m_Objects)
    {
        object->Finalize();
    }
}

void Simulation::AddObject(std::unique_ptr<Object> object)
{
    m_Objects.push_back(std::move(object));
}
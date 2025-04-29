#pragma once

#include <Node/NodeEditor.hpp>
#include <Simulation/Simulation.hpp>

class SimulationEditor : public NodeEditor
{
public:
    SimulationEditor() : NodeEditor("Simulation Editor") {}
    ~SimulationEditor() override {}

    void Update() override { NodeEditor::Update(); }
    void Render() override { NodeEditor::Render(); }

private:
    Simulation m_Simulation;
};
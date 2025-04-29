#pragma once

#include <Node/NodeEditor.hpp>
#include <Simulation/Simulation.hpp>

class SimulationEditor : public NodeEditor
{
public:
    SimulationEditor();
    ~SimulationEditor() override;

    void Update() override;
    void Render() override;

private:
    Simulation m_Simulation;
};
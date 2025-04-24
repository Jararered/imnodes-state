#pragma once

#include <cstdint>

#include <Node/Node.hpp>

#include <string>
#include <cstdint>

class Object : public Node
{
public:
    Object();
    ~Object();

private:
    uint32_t m_InitializeOrder;
    uint32_t m_UpdateOrder;
    uint32_t m_UpdateRate;
};

enum IntegrationMethod
{
    Euler,
    RungeKutta4
};

struct SimulationParameters
{
    double StartTime; // s
    double EndTime;   // s

    uint32_t UpdateRate; // Hz

    IntegrationMethod IntegrationMethod;
};

enum SimulationState
{
    NotInitialized,
    Initialized,
    Running,
    Paused,
    Stopped
};

class Simulation : public NodeLayer
{
public:
    Simulation(const SimulationParameters &parameters);
    ~Simulation();

    void Initialize();
    void Update();
    void UpdateDerivatives();
    void Finalize();

private:
    SimulationState m_State;
    double m_Time;
    std::vector<Object> m_Objects;
    SimulationParameters m_Parameters;
};

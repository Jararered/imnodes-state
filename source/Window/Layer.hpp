#pragma once

#include <string>

class Layer
{
public:
    Layer() = default;
    Layer(const std::string &name) : m_Name(name) {}
    virtual ~Layer() = default;

    virtual void Update() {};
    virtual void Render() {};

    std::string m_Name = "Layer";
};
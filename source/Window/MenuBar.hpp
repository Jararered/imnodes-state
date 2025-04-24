#pragma once

#include "Layer.hpp"

class MenuBar : public Layer
{
public:
    MenuBar() : Layer("Menu Bar") {}
    ~MenuBar() override = default;

    void Update() override {};
    void Render() override;
};

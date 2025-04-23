#pragma once

#include "Layer.hpp"

class MenuBar : public Layer
{
public:
    MenuBar() : Layer("Menu Bar") {}
    ~MenuBar() override = default;

    void Update() override {};
    const void Render() const override;
};

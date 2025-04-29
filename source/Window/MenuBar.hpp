#pragma once

#include "Layer.hpp"

#include <memory>
#include <vector>

class MenuBarMenu
{
public:
    MenuBarMenu(const std::string& name) : m_Name(name) {}
    virtual ~MenuBarMenu() = default;

    virtual void Render() = 0;

protected:
    std::string m_Name;
};

class MenuBar : public Layer
{
public:
    MenuBar();
    ~MenuBar() override = default;

    void Update() override {};
    void Render() override;

    void AddMenu(std::unique_ptr<MenuBarMenu> menu);

private:
    std::vector<std::unique_ptr<MenuBarMenu>> m_MenuBarMenus;
};

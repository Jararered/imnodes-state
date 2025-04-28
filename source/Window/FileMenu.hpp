#pragma once

#include "MenuBar.hpp"

class FileMenu : public MenuBarMenu
{
public:
    FileMenu() : MenuBarMenu("File") {}
    ~FileMenu() override = default;

    void Render() override;
};

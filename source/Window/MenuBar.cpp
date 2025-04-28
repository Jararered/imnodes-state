#include "MenuBar.hpp"

#include "FileMenu.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <iostream>
#include <memory>

MenuBar::MenuBar()
{
    AddMenu(std::make_unique<FileMenu>());
}

void MenuBar::AddMenu(std::unique_ptr<MenuBarMenu> menu)
{
    m_MenuBarMenus.push_back(std::move(menu));
}

void MenuBar::Render()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit", "Ctrl+Q"))
        {
            std::cout << "File Menu: Exit" << std::endl;
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }
        ImGui::EndMenu();
    }

    ImGui::Separator();
    ImGui::Text("%.0f fps", ImGui::GetIO().Framerate);

    ImGui::EndMainMenuBar();
}
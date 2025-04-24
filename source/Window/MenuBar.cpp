#include "MenuBar.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <iostream>

void MenuBar::Render()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New", "Ctrl+N"))
        {
            std::cout << "File Menu: New" << std::endl;
        }
        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {
            std::cout << "File Menu: Open" << std::endl;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {
            std::cout << "File Menu: Save" << std::endl;
        }
        if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        {
            std::cout << "File Menu: Save As" << std::endl;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Ctrl+Q"))
        {
            std::cout << "File Menu: Exit" << std::endl;
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "Ctrl+Z"))
        {
            std::cout << "Edit Menu: Undo" << std::endl;
        }
        if (ImGui::MenuItem("Redo", "Ctrl+Y"))
        {
            std::cout << "Edit Menu: Redo" << std::endl;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "Ctrl+X"))
        {
            std::cout << "Edit Menu: Cut" << std::endl;
        }
        if (ImGui::MenuItem("Copy", "Ctrl+C"))
        {
            std::cout << "Edit Menu: Copy" << std::endl;
        }
        if (ImGui::MenuItem("Rename", "Ctrl+R"))
        {
            std::cout << "Edit Menu: Rename" << std::endl;
        }
        if (ImGui::MenuItem("Paste", "Ctrl+V"))
        {
            std::cout << "Edit Menu: Paste" << std::endl;
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("Toggle Fullscreen", "F11"))
        {
            std::cout << "View Menu: Toggle Fullscreen" << std::endl;
        }
        if (ImGui::MenuItem("Show Statistics"))
        {
            std::cout << "View Menu: Show Statistics" << std::endl;
        }
        if (ImGui::MenuItem("Show Console"))
        {
            std::cout << "View Menu: Show Console" << std::endl;
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("About"))
        {
            std::cout << "Help Menu: About" << std::endl;
        }
        ImGui::EndMenu();
    }

    ImGui::Separator();
    ImGui::Text("%.0f fps", ImGui::GetIO().Framerate);

    ImGui::EndMainMenuBar();
}
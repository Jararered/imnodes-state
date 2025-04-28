#include "FileMenu.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

void FileMenu::Render()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit", "Ctrl+Q"))
        {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }
        ImGui::EndMenu();
    }
}
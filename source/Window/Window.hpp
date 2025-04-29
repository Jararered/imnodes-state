#pragma once

#include "Layer.hpp"

#include <memory>
#include <string>
#include <unordered_map>

struct WindowParameters
{
    std::string Title = "New Window";
    int Width = 1280;
    int Height = 720;
    bool VSync = true;
};

struct GLFWwindow;

class Window
{
public:
    Window(const WindowParameters& parameters);
    ~Window() = default;

    void AddLayer(std::shared_ptr<Layer> layer);
    void RemoveLayer(const std::string& name);

    void Initialize();
    void Update();
    void Finalize();

    void NewFrame();
    void EndFrame();

    void Render();

    bool ShouldClose();

protected:
    void RenderNodeEditor();

private:
    GLFWwindow* m_Window;
    WindowParameters m_Parameters;
    std::unordered_map<std::string, std::shared_ptr<Layer>> m_Layers;
};

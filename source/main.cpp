#include <Window/Window.hpp>
#include <Window/MenuBar.hpp>

#include <Node/NodeLayer.hpp>

int main(void)
{
    WindowParameters parameters;
    parameters.Title = "Node Simulation";
    parameters.Width = 1280;
    parameters.Height = 720;
    parameters.VSync = true;

    Window window(parameters);
    window.Initialize();

    window.AddLayer(std::make_shared<MenuBar>());
    window.AddLayer(std::make_shared<NodeLayer>());
    
    while (!window.ShouldClose())
    {
        window.Update();
    }

    window.Finalize();

    return 0;
}
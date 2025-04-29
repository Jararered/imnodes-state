#include "ObjectPin.hpp"

#include <imgui.h>

ObjectPin::ObjectPin(const ObjectPinParameters& parameters) : m_Parameters(parameters)
{
}

void ObjectPin::Initialize()
{
}

void ObjectPin::Update()
{
}

void ObjectPin::Finalize()
{
}

void ObjectPin::ShowContextMenu()
{
    ImGui::Begin("Object Pin");
    ImGui::Text("Object Pin");
    ImGui::End();
}

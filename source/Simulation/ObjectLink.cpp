#include "ObjectLink.hpp"

#include <imgui.h>

ObjectLink::ObjectLink(const ObjectLinkParameters& parameters) : m_Parameters(parameters)
{
}

void ObjectLink::Initialize()
{
}

void ObjectLink::Update()
{
}

void ObjectLink::Finalize()
{
}

void ObjectLink::ShowContextMenu()
{
    ImGui::Begin("Object Link");
    ImGui::Text("Object Link");
    ImGui::End();
}
#include "ObjectNode.hpp"

#include <imgui.h>

ObjectNode::ObjectNode(const ObjectNodeParameters& parameters) : m_Parameters(parameters)
{
}

void ObjectNode::Initialize()
{
}

void ObjectNode::Update()
{
}

void ObjectNode::Finalize()
{
}

void ObjectNode::ShowContextMenu()
{
    ImGui::Begin("Object Node");
    ImGui::Text("Object Node");
    ImGui::End();
}
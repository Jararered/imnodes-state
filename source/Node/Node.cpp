#include "Node.hpp"

#include <imnodes.h>
#include <imgui.h>

void Node::Render() const
{
    ImNodes::BeginNode(m_Id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(m_Name.c_str());
    ImNodes::EndNodeTitleBar();

    for (const NodeIO &input : m_Inputs)
    {
        ImNodes::BeginInputAttribute(input.m_Id);
        ImGui::TextUnformatted(input.m_Name.c_str());
        ImNodes::EndInputAttribute();
    }

    for (const NodeIO &output : m_Outputs)
    {
        ImNodes::BeginOutputAttribute(output.m_Id);
        ImGui::TextUnformatted(output.m_Name.c_str());
        ImNodes::EndOutputAttribute();
    }

    ImNodes::EndNode();
}

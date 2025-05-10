#pragma once

#include <cstdint>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui.h>

using IDSet = std::set<std::uint32_t>;
using IDQueue = std::queue<std::uint32_t>;

enum Color : unsigned int
{
    Red = IM_COL32(255, 0, 0, 255),
    LightRed = IM_COL32(255, 100, 100, 255),
    LighterRed = IM_COL32(255, 200, 200, 255),
    Orange = IM_COL32(255, 165, 0, 255),
    LightOrange = IM_COL32(255, 200, 100, 255),
    LighterOrange = IM_COL32(255, 255, 100, 255),
    Yellow = IM_COL32(255, 255, 0, 255),
    LightYellow = IM_COL32(255, 255, 100, 255),
    LighterYellow = IM_COL32(255, 255, 200, 255),
    Lime = IM_COL32(191, 255, 0, 255),
    LightLime = IM_COL32(200, 255, 100, 255),
    LighterLime = IM_COL32(255, 255, 200, 255),
    Green = IM_COL32(0, 255, 0, 255),
    LightGreen = IM_COL32(100, 255, 100, 255),
    LighterGreen = IM_COL32(200, 255, 200, 255),
    Cyan = IM_COL32(0, 255, 255, 255),
    LightCyan = IM_COL32(100, 255, 255, 255),
    LighterCyan = IM_COL32(200, 255, 255, 255),
    Blue = IM_COL32(0, 0, 255, 255),
    LightBlue = IM_COL32(100, 100, 255, 255),
    LighterBlue = IM_COL32(200, 200, 255, 255),
    Purple = IM_COL32(128, 0, 128, 255),
    LightPurple = IM_COL32(200, 100, 255, 255),
    LighterPurple = IM_COL32(255, 200, 255, 255),
    Magenta = IM_COL32(255, 0, 255, 255),
    LightMagenta = IM_COL32(255, 100, 255, 255),
    LighterMagenta = IM_COL32(255, 200, 255, 255),
    Gray = IM_COL32(128, 128, 128, 255),
    LightGray = IM_COL32(200, 200, 200, 255),
    LighterGray = IM_COL32(255, 255, 255, 255),
};


struct ColorData
{
    unsigned int NormalColor = IM_COL32(41, 74, 122, 255);
    unsigned int HoverColor = IM_COL32(50, 85, 135, 255);
    unsigned int SelectedColor = IM_COL32(66, 150, 250, 255);
};

struct NodePosition
{
    float X = 0.0f;
    float Y = 0.0f;
};

const std::unordered_map<std::string, ColorData> ColorDataPresets = {
    {"Default", ColorData{IM_COL32(41, 74, 122, 255), IM_COL32(50, 85, 135, 255), IM_COL32(66, 150, 250, 255)}},
    {"Red", {Color::Red, Color::LightRed, Color::LighterRed}},
    {"Orange", {Color::Orange, Color::LightOrange, Color::LighterOrange}},
    {"Yellow", {Color::Yellow, Color::LightYellow, Color::LighterYellow}},
    {"Lime", {Color::Lime, Color::LightLime, Color::LighterLime}},
    {"Green", {Color::Green, Color::LightGreen, Color::LighterGreen}},
    {"Cyan", {Color::Cyan, Color::LightCyan, Color::LighterCyan}},
    {"Blue", {Color::Blue, Color::LightBlue, Color::LighterBlue}},
    {"Purple", {Color::Purple, Color::LightPurple, Color::LighterPurple}},
    {"Magenta", {Color::Magenta, Color::LightMagenta, Color::LighterMagenta}},
    {"Gray", {Color::Gray, Color::LightGray, Color::LighterGray}},
};

enum class PinType : std::uint8_t
{
    Input = 0,
    Output = 1
};

struct PinData
{
    std::string Name;
    std::uint32_t NodeID;
    PinType Type;
    IDSet Links;
    ColorData Colors;

    void ShowToolTip()
    {
        ImGui::BeginTooltip();
        ImGui::Text("Node ID: %d", NodeID);
        ImGui::Text("Type: %s", Type == PinType::Input ? "Input" : "Output");
        ImGui::Text("Links: %d", Links.size());
        ImGui::EndTooltip();
    }
};

struct NodeData
{
    std::string Name;
    NodePosition GridPosition;
    IDSet InputIDs;
    IDSet OutputIDs;
    ColorData Colors;
};

struct LinkData
{
    std::uint32_t Pin1ID = 0;
    std::uint32_t Pin2ID = 0;
    ColorData Colors;
};

using PinDataMap = std::unordered_map<std::uint32_t, PinData>;
using NodeDataMap = std::unordered_map<std::uint32_t, NodeData>;
using LinkDataMap = std::unordered_map<std::uint32_t, LinkData>;
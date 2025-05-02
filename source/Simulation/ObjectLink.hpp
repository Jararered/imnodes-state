#pragma once

#include <Node/NodeTypes.hpp>

enum class ObjectLinkType : std::uint8_t
{
    Unset = 0,
    Float = 1,
    Int = 2,
    Bool = 3,
};

struct ObjectLinkParameters
{
    ObjectLinkType Type = ObjectLinkType::Unset;
};

class ObjectLink : public LinkData
{
public:
    ObjectLink() = default;
    ObjectLink(const ObjectLinkParameters& parameters);
    ~ObjectLink() = default;

    virtual void Initialize();
    virtual void Update();
    virtual void Finalize();

    virtual void ShowContextMenu();

private:
    ObjectLinkParameters m_Parameters;
};

#pragma once

#include <Node/NodeTypes.hpp>

enum class ObjectPinType : uint8_t
{
    Unset = 0,
    Float = 1,
    Int = 2,
    Bool = 3,
};

struct ObjectPinParameters
{
    ObjectPinType Type = ObjectPinType::Unset;
};

class ObjectPin : public PinData
{
public:
    ObjectPin() = default;
    ObjectPin(const ObjectPinParameters& parameters);
    ~ObjectPin() = default;

    virtual void Initialize();
    virtual void Update();
    virtual void Finalize();

    virtual void ShowContextMenu();

private:
    ObjectPinParameters m_Parameters;
};

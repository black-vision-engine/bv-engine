#pragma once

#include "Engine/Models/Plugin.h"

#include "Solid.h" // FIXME: move ParametersDescriptor to separated header

namespace bv
{

class ParamFloat;
class ValueFloat;

class ExtrudeParamDescriptor : public BaseParametersDescriptor 
{
public:
    std::string m_scaleParamName;

    ExtrudeParamDescriptor();
};

class ExtrudePlugin : public BasePlugin< IShaderPlugin >
{
    ExtrudeParamDescriptor*     m_paramDesc;

    ParamFloat*                 m_scale;
    ValueFloat*                 m_scaleValue;

public:

    virtual std::string     GetShaderFile() const override;

    void                    Update(float t) override;

    explicit                ExtrudePlugin(const FloatInterpolator& scale);

    virtual void            Print(std::ostream &,int) const {}
};

} // bv
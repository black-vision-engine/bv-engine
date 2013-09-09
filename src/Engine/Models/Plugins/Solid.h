#pragma once 

#include "Engine/Models/Plugin.h"

#include <hash_map>

namespace bv {

class BuildScene;
class ParamVec4;
class ValueVec4;

class IParametersDescriptor
{
public:
    typedef std::hash_map<std::string, ParamType>  StringParamTypeMap;

    virtual const StringParamTypeMap&               GetParams()             const = 0;

    virtual bool                                    ValidateParameters(const std::string& programSource)    const = 0;

    virtual ~IParametersDescriptor(){}
};

class BaseParametersDescriptor : public IParametersDescriptor
{
protected:
    StringParamTypeMap                          m_params;

public:
    const StringParamTypeMap&                   GetParams()     const { return m_params; }

    bool                                        ValidateParameters(const std::string& programSource)    const { return true; } //FIXME: Add validation

    virtual ~BaseParametersDescriptor(){}
};

class SolidParametersDescriptor : public BaseParametersDescriptor
{
public:
    std::string m_colorParamName;

    explicit SolidParametersDescriptor();
};

class SolidColorPlugin : public BasePlugin< IShaderPlugin >
{
private:
    ParamVec4*                  m_color;
    ValueVec4*                  m_colorValue;
    SolidParametersDescriptor*  m_paramDesc;
public:
    virtual std::string     GetShaderFile() const override;

    explicit SolidColorPlugin(const Vec4Interpolator& col);

    void    Update(float t) override;
    void    Print(std::ostream& out, int tabs = 0) const override;
};

} // bv
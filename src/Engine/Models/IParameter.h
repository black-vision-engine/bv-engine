#pragma once

#include <string>
#include "Mathematics/Interpolators/Interpolators.h"
#include "Engine/Graphics/Shaders/ShaderTypes.h"

namespace bv {

class IParameter
{
protected:
    std::string         m_name;
    ParameterSemantic   m_semantic;
public:

    virtual ParamType            GetParamType() const = 0;


    std::string         getName()       const { return m_name; }
    ParameterSemantic   getSemantic()   const { return m_semantic; }

    IParameter(const std::string& name, ParameterSemantic semantic)
        : m_name(name)
        , m_semantic(semantic)
    {}

    virtual ~IParameter(){}
};

class IValue
{
public:
    virtual ParamType           GetParamType    ()              const = 0;
    virtual const char*         GetData         ()              const = 0;
    virtual const std::string&  GetName         ()              const = 0;  

    virtual ~IValue(){}
};

class BaseValue : public IValue
{
private:
    std::string             m_name;

protected:
    explicit BaseValue(const std::string& name)
        : m_name(name)
    {}

    virtual ~BaseValue(){}

public:
    virtual const std::string&  GetName     ()              const { return m_name; }
};

}
#pragma once

#include <string>
#include "Mathematics/Interpolators/Interpolators.h"
#include "Engine/Graphics/Shaders/ShaderTypes.h"
#include "Engine/Types/Enums.h"

namespace bv { namespace model {

class IParameter
{
public:

    virtual ParamType           GetParamType() const = 0;


    virtual std::string         GetName()       const = 0;
    virtual ParameterSemantic   GetSemantic()   const = 0;

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

} // model
} // bv
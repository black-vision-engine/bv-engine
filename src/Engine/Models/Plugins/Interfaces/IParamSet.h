#pragma once

#include <vector>

namespace bv { namespace model {

class IParameter;

class IParamSet
{
public:

    virtual std::vector< IParameter * > &       GetParameters       () = 0;

    virtual ~IParamSet(){}

};

} //model
} //bv

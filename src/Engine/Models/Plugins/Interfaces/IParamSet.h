#pragma once

#include <vector>

namespace bv { namespace model {

class IParameter;

class IParamSet
{
public:

    virtual std::vector< IParameter * > &       QueryParameters     () = 0;

    virtual ~IParamSet(){}

};

} //model
} //bv

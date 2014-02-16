#pragma once

#include <vector>

namespace bv { namespace model {

class IValue;

class IValueSet
{
public:

    virtual std::vector< IValue * > &       QueryValues () = 0;

    virtual ~IValueSet(){}

};

} //model
} //bv

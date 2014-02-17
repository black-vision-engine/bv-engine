#pragma once

#include <vector>

namespace bv { namespace model {

class IValue;

class IValueSet
{
public:

    virtual std::vector< IValue * > &       GetValues   () = 0;
    virtual IValue *                        GetValue    ( const std::string & name ) = 0;

    virtual ~IValueSet(){}

};

} //model
} //bv

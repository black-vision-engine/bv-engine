#pragma once

#include <vector>

namespace bv { namespace model {

class IValue;

class IValueSet
{
public:

    virtual const std::vector< IValue * > & GetValues   () const = 0;
    virtual IValue *                        GetValue    ( const std::string & name ) const = 0;

    virtual ~IValueSet(){}

};

} //model
} //bv

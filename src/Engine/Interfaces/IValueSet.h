#pragma once

#include <vector>
#include "Engine/Interfaces/IValue.h"


namespace bv {

class IValueSet
{
public:

    virtual const std::vector< IValue * > & GetValues   () const = 0;
    virtual IValue *                        GetValue    ( const std::string & name ) const = 0;

    virtual ~IValueSet(){}

};

} //bv

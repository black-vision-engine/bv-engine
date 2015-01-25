#pragma once

#include <vector>
#include "Engine/Interfaces/IValue.h"

namespace bv {

class IValueSet
{
public:

    virtual const std::vector< IValueConstPtr > &   GetValues   () const = 0;
    virtual IValueConstPtr                          GetValue    ( const std::string & name ) const = 0;

    virtual ~IValueSet(){}

};

DEFINE_PTR_TYPE(IValueSet)
typedef std::shared_ptr< const IValueSet > IValueSetConstPtr;

} //bv

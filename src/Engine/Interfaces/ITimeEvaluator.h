#pragma once

#include <string>
#include "System/BasicTypes.h" //FIXME: store in PCH


namespace bv { namespace model {

class ITimeEvaluator
{
public:

    virtual const std::string & GetName         () const                = 0;
    virtual TimeType            Evaluate        ( TimeType t ) const    = 0;

    virtual                     ~ITimeEvaluator () {}

};

} //model
} //bv

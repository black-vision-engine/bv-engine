#pragma once

#include <string>
#include <memory>

#include "System/BasicTypes.h" //FIXME: store in PCH


namespace bv { namespace model {

class ITimeEvaluator
{
public:

    virtual const std::string & GetName         () const        = 0;

    virtual void                SetGlobalTime   ( TimeType t )  = 0;
    virtual TimeType            GetLocalTime    () const        = 0;

    virtual                     ~ITimeEvaluator () {}
};

typedef std::shared_ptr< ITimeEvaluator > ITimeEvaluatorPtr;

} //model
} //bv

#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ITimeline : public ITimeEvaluator
{
public:

    virtual void                Start           () = 0;
    virtual void                Stop            () = 0;
    
    virtual                     ~ITimeline      () {}

};

} //model
} //bv

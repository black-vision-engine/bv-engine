#pragma once

namespace bv { namespace model {

class ISequenceAnimationSource
{
public:

    virtual unsigned int    CurrentFrame                ( TimeType t ) const = 0;
    virtual unsigned int    PredictedNextFrame          ( TimeType t ) const = 0; //Means next index of frame on when needed not during next update

    virtual                 ~ISequenceAnimationSource   () {}

};

} //model
} //bv

#pragma once

#include "TranformChannel.h"
#include "Mathematics\Transform\MatTransform.h"

namespace bv { namespace model
{

class SimpleTransformChannel : public TransformChannel
{
private:
    std::vector< TransformF* >                      m_transformInterpolators;

public:
    virtual void                                    Update( float t );

    void                                            AddTransformChannel( TransformF* transInt );

};

} // model
} // bv
#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"


namespace bv { namespace model
{

class SimpleTransformChannel : public TransformChannel
{
private:

    std::vector< TransformF* >                      m_transformInterpolators;

public:

    virtual void                                    Update( float t );

    void                                            AddTransform( TransformF* transInt );

};

} // model
} // bv
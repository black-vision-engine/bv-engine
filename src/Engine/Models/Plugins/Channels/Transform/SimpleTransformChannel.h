#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"

namespace bv { namespace model
{

class SimpleTransformChannel : public TransformChannel
{
private:

    std::vector< ParamTransform >                   m_transformParams;

public:

    virtual void                                    Update( TimeType t );

    void                                            AddTransform( const ParamTransform& transInt );

};

} // model
} // bv
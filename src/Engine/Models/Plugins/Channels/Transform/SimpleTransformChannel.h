#pragma once

#include <memory>

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Parameters/TypedParameters.h"

namespace bv { namespace model
{

class SimpleTransformChannel : public TransformChannel
{
private:

    std::vector< ParamTransform >                   m_transformParams;

public:

    virtual void                                    Update                  ( TimeType t );

    void                                            AddTransform            ( const ParamTransform & transInt );

    ParamTransform &                                AccessFirstParamTransform() //FIXME: hackish
    {
        return m_transformParams[ 0 ];
    }
};

typedef std::shared_ptr< SimpleTransformChannel >   SimpleTransformChannelPtr;

} // model
} // bv
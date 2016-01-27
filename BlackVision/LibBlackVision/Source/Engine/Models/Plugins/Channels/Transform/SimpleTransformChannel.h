#pragma once

#include <memory>

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"

namespace bv { namespace model
{

class SimpleTransformChannel : public TransformChannel
{
private:

    ParamTransform      m_transformParam;

public:

    explicit            SimpleTransformChannel  ( const ParamTransform & paramTransform );

    virtual void        Update                  ( TimeType t );

//    void                                            AddTransform            ( const ParamTransform & transInt );

    ParamTransform *    GetParamTransform    ();

};

DEFINE_PTR_TYPE(SimpleTransformChannel)

} // model
} // bv
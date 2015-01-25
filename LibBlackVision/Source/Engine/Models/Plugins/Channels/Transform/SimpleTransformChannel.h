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

    ParamTransformVec   m_transformParams;

public:

    explicit            SimpleTransformChannel  ( const ParamTransformVec & paramTransformVec );

    virtual void        Update                  ( TimeType t );

//    void                                            AddTransform            ( const ParamTransform & transInt );

    ParamTransformVec * GetParamTransformVec    ();

};

DEFINE_PTR_TYPE(SimpleTransformChannel)

} // model
} // bv
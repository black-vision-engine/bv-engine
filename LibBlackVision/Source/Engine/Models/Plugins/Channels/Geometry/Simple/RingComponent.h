#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"


namespace bv { namespace model
{

class RingComponent;
DEFINE_PTR_TYPE(RingComponent)
DEFINE_CONST_PTR_TYPE(RingComponent)

class RingComponent : public ConnectedComponent
{
private:

                            RingComponent   ( float startAngle, float endAngle, float innerRadius, float outerRadiusoat, int fragmentsNum );

public:

    static RingComponentPtr Create          ( float startAngle, float endAngle, float innerRadius, float outerRadiusoat, int fragmentsNum );

};

} // model
} // bv

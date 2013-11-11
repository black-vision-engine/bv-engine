#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"


namespace bv { namespace model
{

class RingComponent : public ConnectedComponent
{
private:

                            RingComponent   ( float startAngle, float endAngle, float innerRadius, float outerRadiusoat, int fragmentsNum );

public:

    static RingComponent *  Create          ( float startAngle, float endAngle, float innerRadius, float outerRadiusoat, int fragmentsNum );

};

} // model
} // bv

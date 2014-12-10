#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"


// ************************************************************************* COMPONENT *************************************************************************

namespace bv { namespace model
{

class PrismComponent;
DEFINE_PTR_TYPE(PrismComponent)
DEFINE_CONST_PTR_TYPE(PrismComponent)

class PrismComponent : public ConnectedComponent
{
private:

                            PrismComponent   ( /*float startAngle, float endAngle, float innerRadius, float outerRadiusoat, int fragmentsNum*/ );

public:

    static std::pair< ConnectedComponentPtr, ConnectedComponentPtr > Create          ( /*float startAngle, float endAngle, float innerRadius, float outerRadiusoat,*/ int fragmentsNum );

};

// ************************************************************************* DESCRIPTOR *************************************************************************



} // model
} // bv

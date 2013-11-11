#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"


namespace bv { namespace model
{

class RectComponent : public ConnectedComponent
{
private:

                            RectComponent   ( float w, float h, float tx, float ty, float tz );

public:

    static RectComponent *  Create          ( float w = 1.f, float h = 1.f, float tx = 0.0f, float ty = 0.0f, float tz = 0.0f );

};

} // model
} // bv

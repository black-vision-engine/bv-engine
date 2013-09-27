#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

namespace bv { namespace model
{

class RectComponent : public ConnectedComponent
{
private:

    RectComponent               ( float w, float h );

public:

    static RectComponent *              Create                      ( float w = 1.f, float h = 1.f );

};

} // model
} // bv

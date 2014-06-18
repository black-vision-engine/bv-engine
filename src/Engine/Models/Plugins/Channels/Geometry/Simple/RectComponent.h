#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model
{

class RectComponent : public ConnectedComponent
{
private:

    Float3AttributeChannelPtr    m_v3Attrs;

private:

                            RectComponent   ( float w, float h, float tx, float ty, float tz );

public:

    void                    SetRectSize     ( float w, float h );

    static RectComponent *  Create          ( float w = 1.f, float h = 1.f, float tx = 0.0f, float ty = 0.0f, float tz = 0.0f );

};

} // model
} // bv

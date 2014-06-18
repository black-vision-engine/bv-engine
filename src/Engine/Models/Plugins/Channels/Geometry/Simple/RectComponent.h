#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model
{

class RectComponent;
DEFINE_PTR_TYPE(RectComponent)
DEFINE_CONST_PTR_TYPE(RectComponent)

class RectComponent : public ConnectedComponent
{
private:

    Float3AttributeChannelPtr    m_v3Attrs;

private:

                            RectComponent   ( float w, float h, float tx, float ty, float tz );

public:

    void                    SetRectSize     ( float w, float h );

    static RectComponentPtr Create          ( float w = 1.f, float h = 1.f, float tx = 0.0f, float ty = 0.0f, float tz = 0.0f );

};

} // model
} // bv

#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {

class AnimatedStripComponent;
DEFINE_PTR_TYPE(AnimatedStripComponent)
DEFINE_CONST_PTR_TYPE(AnimatedStripComponent)

class AnimatedStripComponent: public ConnectedComponent
{
private:

    float   m_speedX;
    float   m_speedY;
    float   m_sclSine;
    float   m_sclCosine;
    float   m_h;
    float   m_z;
    float   m_sizeY;
    float   m_sizeZ;

    Float3AttributeChannelPtr           m_positions;

private:

                                        AnimatedStripComponent  ( float w, float h, unsigned int numSegments, float z, float sclSine, float sclCosine, float speedX, float speedY, float sizeY, float sizeZ );

public:
                                        ~AnimatedStripComponent ();

    virtual void                        Update                  ( TimeType t );

    static  AnimatedStripComponentPtr   Create                  ( float w, float h, unsigned int numSegments, float z, float sclSine, float sclCosine, float speedX, float speedY, float sizeY, float sizeZ );

};

} //model
} //bv

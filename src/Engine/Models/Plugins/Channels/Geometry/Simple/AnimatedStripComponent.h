#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"


namespace bv { namespace model {

class Float3AttributeChannel;

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

    Float3AttributeChannel *    m_positions;

private:

                                        AnimatedStripComponent  ( float w, float h, unsigned int numSegments, float z, float sclSine, float sclCosine, float speedX, float speedY, float sizeY, float sizeZ );
                                        ~AnimatedStripComponent ();

public:

    virtual void                        Update                  ( TimeType t );

    static  AnimatedStripComponent *    Create                  ( float w, float h, unsigned int numSegments, float z, float sclSine, float sclCosine, float speedX, float speedY, float sizeY, float sizeZ );

};

} //model
} //bv

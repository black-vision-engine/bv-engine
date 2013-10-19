#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

namespace bv { namespace model {

class AnimatedStripComponent: public ConnectedComponent
{
private:

            AnimatedStripComponent                  ( float w, float h, unsigned int numSegments, float z );
            ~AnimatedStripComponent                 ();

public:

    static  AnimatedStripComponent *  Create        ( float w, float h, unsigned int numSegments, float z );

};

} //model
} //bv

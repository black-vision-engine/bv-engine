#pragma once

#include "CoreDEF.h"

#include "BVGL.h"
#include "Engine/Graphics/Renderers/Renderer.h"

namespace bv {

class IBasicLogic
{
public:
    virtual     void    Initialize          ( Renderer * renderer ) = 0;

    virtual     void    Render              ()                      = 0;
    virtual     void    Update              ( TimeType t )          = 0;
    virtual     void    Key                 ( unsigned char c )     = 0;

    virtual     void    Resize              ( UInt32 w, UInt32 h )  = 0;

    virtual             ~IBasicLogic		()						= 0 {}
};

} // bv

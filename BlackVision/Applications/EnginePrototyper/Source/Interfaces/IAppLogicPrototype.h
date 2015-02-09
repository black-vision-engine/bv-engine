#pragma once

#include "Common/PrototyperCore.h"


namespace bv {

class IAppLogicPrototype
{
public:

    // Load textures, initialize shaders, set up parameters, etc.
    virtual     void    Initialize          ()                      = 0;

    // Update method called each frame prior to rendering (parameter update, camera setup and so son)
    virtual     void    Update              ( TimeType t )          = 0;

    // Method responsible for rendering
    virtual     void    Render              ()                      = 0;

    // Method responsible for servicing keyboard events
    virtual     void    Key                 ( unsigned char c )     = 0;

    // Usual resize
    virtual     void    Resize              ( UInt32 w, UInt32 h )  = 0;

    virtual             ~IAppLogicPrototype () = 0 {}
};

} // bv

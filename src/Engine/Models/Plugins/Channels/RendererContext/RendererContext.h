#pragma once

#include "Engine/Models/Plugins/Channels/RendererContext/AlphaContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/CullContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/DepthContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/FillContext.h"


namespace bv { namespace model {

class RendererContext
{
private:

    mutable bool    m_stateChanged;

public:

    AlphaContext *  alphaCtx;
    CullContext  *  cullCtx;
    DepthContext *  depthCtx;
    FillContext  *  fillCtx;

public:

            RendererContext     ();
            ~RendererContext    ();


    bool    StateChanged        () const;
    void    SetStateChanged     ( bool stateChanged ) const;

};

} //model
} //bv

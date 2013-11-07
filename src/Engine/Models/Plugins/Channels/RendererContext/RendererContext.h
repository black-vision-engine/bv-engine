#pragma once

#include "Engine/Models/Plugins/Channels/RendererContext/AlphaContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/CullContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/DepthContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/FillContext.h"

namespace bv { namespace model {

class RendererContext
{
public:

    AlphaContext * alphaCtx;
    CullContext  * cullCtx;
    DepthContext * depthCtx;
    FillContext  * fillCtx;

public:

    RendererContext     ();
    ~RendererContext    ();
};

} //model
} //bv

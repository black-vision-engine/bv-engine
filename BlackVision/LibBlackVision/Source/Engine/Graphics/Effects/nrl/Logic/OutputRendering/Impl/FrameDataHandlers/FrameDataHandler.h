#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectComponentState.h"


namespace bv { namespace nrl {
   
class NOutputState;
class RenderChannel;
class NRenderContext;

class FrameDataHandler
{
private:

public:

    virtual                                     ~FrameDataHandler   ();

    virtual void                                HandleFrameData     ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * channel ) = 0;

    virtual NFullscreenEffectComponentStatePtr  GetInternalFSEState () = 0;

};

} //nrl
} //bv

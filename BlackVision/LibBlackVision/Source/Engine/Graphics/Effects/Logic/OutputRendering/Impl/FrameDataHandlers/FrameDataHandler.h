#pragma once

#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectComponentState.h"


namespace bv { 
   
class OutputState;
class RenderChannel;
class RenderContext;

class FrameDataHandler
{
private:

public:

    virtual                                     ~FrameDataHandler   ();

    virtual void                                HandleFrameData     ( const OutputState & state, RenderContext * ctx, const RenderChannel * channel ) = 0;

    virtual FullscreenEffectComponentStatePtr  GetInternalFSEState () = 0;

};


} //bv

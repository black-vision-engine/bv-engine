#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffect.h"


namespace bv { namespace nrl {
   
class PreviewHandler : public FrameDataHandler
{
private:

    //FIXME: this is the most basic logic. More advanced implementation would allow additional processing of RenderResult (see ShowFrame)
    NRenderedData           m_activeRenderOutput;

    NFullscreenEffect *     m_mixChannelsEffect;

public:

                                                PreviewHandler      ();
                                                ~PreviewHandler     ();

    virtual void                                HandleFrameData     ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * channel ) override;

    virtual NFullscreenEffectComponentStatePtr  GetInternalFSEState () override;

};

} //nrl
} //bv

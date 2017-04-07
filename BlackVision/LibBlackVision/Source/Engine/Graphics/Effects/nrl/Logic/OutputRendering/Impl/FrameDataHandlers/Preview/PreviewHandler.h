#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/RenderedData.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/FullscreenEffect.h"


namespace bv { namespace nrl {
   
class PreviewHandler : public FrameDataHandler
{
private:

    //FIXME: this is the most basic logic. More advanced implementation would allow additional processing of RenderResult (see ShowFrame)
    RenderedData           m_activeRenderOutput;

    FullscreenEffect *     m_mixChannelsEffect;

public:

                                                PreviewHandler      ();
                                                ~PreviewHandler     ();

    virtual void                                HandleFrameData     ( const OutputState & state, RenderContext * ctx, const RenderChannel * channel ) override;

    virtual FullscreenEffectComponentStatePtr  GetInternalFSEState () override;

};

} //nrl
} //bv

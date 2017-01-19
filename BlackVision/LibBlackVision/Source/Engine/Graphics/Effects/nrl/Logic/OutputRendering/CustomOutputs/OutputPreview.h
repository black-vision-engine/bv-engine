#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"


namespace bv { 
    
class RenderTarget;

namespace nrl {

class NFullscreenEffect;
class RenderResult;
class NRenderContext;
 
class OutputPreview : public OutputInstance
{
private:

    //FIXME: this is the most basic logic. More advanced implementation would allow additional processing of RenderResult (see ShowFrame)
    NRenderedData           m_activeRenderOutput;

    NFullscreenEffect *     m_activeBlit;

    NFullscreenEffect *     m_defaultBlitEffect;
    NFullscreenEffect *     m_blitEffectShowAplhaRGB;

public:

                    OutputPreview   ( unsigned int width, unsigned int height );
                    ~OutputPreview  ();

    virtual void    ProcessFrameData( NRenderContext * ctx, RenderResult * result ) override;

    void            SetShowDefault  ();
    void            SetShowAlpha    ();

private:

    void    DefaultShow     ( NRenderContext * ctx, const RenderTarget * rt );

};

} //nrl
} //bv

#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"


namespace bv { namespace nrl {
 
class OutputPreview : public Output
{
private:

    //FIXME: this is the most basic logic. More advanced implementation would allow additional processing of RenderResult (see ShowFrame)
    NRenderedData           m_activeRenderOutput;

    NFullscreenEffect *     m_mixChannelsEffect;

public:

                    OutputPreview   ( unsigned int width, unsigned int height );
                    ~OutputPreview  ();

    virtual void    ProcessFrameData( NRenderContext * ctx, RenderResult * input ) override;

private:

    void    DefaultShow             ( NRenderContext * ctx, const RenderTarget * rt );

    void    UpdateEffectValues      ();

};

} //nrl
} //bv

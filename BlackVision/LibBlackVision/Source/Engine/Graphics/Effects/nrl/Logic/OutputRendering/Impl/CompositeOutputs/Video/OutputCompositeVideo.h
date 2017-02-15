#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NOutputState.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoOutputsHandler.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoOutputsPreprocessor.h"

namespace bv { namespace nrl {

class FrameDataHandler;

class OutputCompositeVideo : public Output
{
private:

    unsigned int                m_mainWidth;
    unsigned int                m_mainHeight;

    VideoOutputsPreprocessor    m_outputsPreprocessor;
    VideoOutputsHandler         m_outputsHandler;
    
public:

                        OutputCompositeVideo    ( unsigned int width, unsigned int height );
                        ~OutputCompositeVideo   ();

    virtual void        ProcessFrameData        ( NRenderContext * ctx, RenderedChannelsData * input ) override;


};

} //nrl
} //bv

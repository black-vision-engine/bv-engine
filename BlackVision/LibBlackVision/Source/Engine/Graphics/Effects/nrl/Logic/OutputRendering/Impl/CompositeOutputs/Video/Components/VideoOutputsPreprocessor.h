#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/AVOutputsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannelsData.h"

#include "AVFrame.h"


namespace bv { 

namespace audio {
    
class AudioRenderer;

}

namespace nrl {

class VideoOutputsPreprocessor
{
private:

    bool                        m_initialized;

    AVOutputsData               m_avFrames;

    VideoInputChannelsData      m_inputChannels;

    UInt32                      m_lcmFPS;

public:

                            VideoOutputsPreprocessor();

    const AVOutputsData &   Preprocess              ( NRenderContext * ctx, RenderedChannelsData * input );

    void                    InvalidateCachedData    ();

    void                    Initialize              ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping );

private:

    AVFramePtr              PrepareAVFrame          ( NRenderContext * ctx, const VideoInputChannel * channel );

};

} //bv
} //nrl

#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/AVOutputsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannelsData.h"

#include "AVFrame.h"

#include <vector>
#include <boost/circular_buffer.hpp>

namespace bv { 

namespace audio {
    
class AudioRenderer;

}

namespace nrl {

class VideoOutputsPreprocessor
{
private:
    typedef std::map< const VideoInputChannel *, boost::circular_buffer< AVFramePtr > > AVFrameBuffersMap;
    typedef std::map< const VideoInputChannel *, AVFramePtr >                           AVFramesMap;

    bool                        m_initialized;

    AVOutputsData               m_avFrames;

    VideoInputChannelsData      m_inputChannels;

    UInt32                      m_lcmFPS;

    AVFrameBuffersMap           m_avFramesBuffer;
    AVFramesMap                 m_currentAVFrames;

public:

                            VideoOutputsPreprocessor();

    const AVOutputsData &   Preprocess              ( NRenderContext * ctx, RenderedChannelsData * input );

    void                    InvalidateCachedData    ();

    void                    Initialize              ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping );

private:

    AVFramePtr              PrepareAVFrame          ( NRenderContext * ctx, const VideoInputChannel * channel );

    void                    InitializeAVBuffers     ( NRenderContext * ctx );

};

} //bv
} //nrl

#pragma once

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/AVOutputsData.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannelsData.h"

#include "Memory/AVFrame.h"

#include <vector>
#include <boost/circular_buffer.hpp>

namespace bv { 

namespace audio {
    
class AudioRenderer;
class AudioMixer;

}

class VideoOutputsPreprocessor
{
    FRIEND_TEST_ACCESSOR( VideoOutputsPreprocessor )
private:
    typedef std::map< const VideoInputChannel *, boost::circular_buffer< AVFramePtr > > AVFrameBuffersMap;
    typedef std::map< const VideoInputChannel *, AVFramePtr >                           AVFramesMap;

    bool                        m_initialized;

    AVOutputsData               m_avFrames;

    VideoInputChannelsData      m_inputChannels;

    UInt32                      m_lcmFPS;

    AVFrameBuffersMap           m_avFramesBuffer;
    AVFramesMap                 m_currentAVFrames;

    std::map< const VideoInputChannel *, audio::AudioMixer * >        m_audioMixers;

public:

                            VideoOutputsPreprocessor    ();
    virtual                 ~VideoOutputsPreprocessor   ();

    const AVOutputsData &   Preprocess              ( RenderContext * ctx, RenderedChannelsData * input );

    void                    InvalidateCachedData    ();

    void                    Initialize              ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping );

private:

    AVFramePtr              PrepareAVFrame          ( RenderContext * ctx, const VideoInputChannel * channel );

    void                    PostInitialize          ( RenderContext * ctx, RenderedChannelsData * input );
    void                    InitializeAVBuffers     ( RenderContext * ctx );

};

} //bv

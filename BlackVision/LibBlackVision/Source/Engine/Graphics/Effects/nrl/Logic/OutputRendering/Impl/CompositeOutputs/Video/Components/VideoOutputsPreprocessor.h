#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/AVOutputsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannelsData.h"


namespace bv { namespace nrl {

class VideoOutputsPreprocessor
{
private:

    bool                        m_initialized;

    AVOutputsData               m_avFrames;

    VideoInputChannelsData      m_inputChannels;

public:

                            VideoOutputsPreprocessor();

    const AVOutputsData &   Preprocess              ( RenderedChannelsData * input );

    void                    InvalidateCachedData    ();

    void                    Initialize              ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping );

};

} //bv
} //nrl

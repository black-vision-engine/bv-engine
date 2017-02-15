#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/AVOutputsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannelsData.h"


namespace bv { namespace nrl {

class VideoOutputsPreprocessor
{
private:

    AVOutputsData               m_avFrames;

    VideoInputChannelsData *    m_inputChannels;

public:

    const AVOutputsData &   Preprocess          ( RenderedChannelsData * input ) const;

    void                    InvalidateCachedData();

};

} //bv
} //nrl

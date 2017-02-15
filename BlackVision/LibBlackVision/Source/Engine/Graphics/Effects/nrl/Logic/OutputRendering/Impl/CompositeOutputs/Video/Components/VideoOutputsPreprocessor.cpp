#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"


namespace bv { namespace nrl {

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::Preprocess          ( RenderedChannelsData * input ) const
{
    { input; }

    return m_avFrames;
}

// *********************************
//
void                    VideoOutputsPreprocessor::InvalidateCachedData()
{
    // FIXME: implement
}

} //bv
} //nrl

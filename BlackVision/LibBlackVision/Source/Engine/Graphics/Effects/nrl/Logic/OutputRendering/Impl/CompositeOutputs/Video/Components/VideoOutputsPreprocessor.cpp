#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"


namespace bv { namespace nrl {

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::Preprocess            ( RenderedChannelsData * input ) const
{
    { input; }

    return m_avFrames;
}

// *********************************
//
void                    VideoOutputsPreprocessor::InvalidateCachedData  ()
{
    // FIXME: implement
}

// *********************************
//
void                    VideoOutputsPreprocessor::PreInitialize         ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{
    { uniqueOutputSetups; }
    { mapping; }
}

} //bv
} //nrl

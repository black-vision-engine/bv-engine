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
    assert( m_avFrames.GetNumEntries() == 0 );    

    { uniqueOutputSetups; }

    for( auto & a : mapping )
    {
        m_avFrames.SetAVFrame( a.first, nullptr );
    }


    m_inputChannels.PreInitialize( 
}

} //bv
} //nrl

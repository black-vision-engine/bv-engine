#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"


namespace bv { namespace nrl {

// *********************************
//
VideoOutputsPreprocessor::VideoOutputsPreprocessor()
    : m_initialized( false )
{
}

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::Preprocess            ( RenderedChannelsData * input )
{
    if( !m_initialized )
    {
        m_inputChannels.PostInitialize( input );
    
        m_initialized = true;
    }

    m_inputChannels
    // FIXME: nrl - implement
    return m_avFrames;
}

// *********************************
//
void                    VideoOutputsPreprocessor::InvalidateCachedData  ()
{
    if( m_initialized )
    {
        // FIXME: implement
    }
}

// *********************************
//
void                    VideoOutputsPreprocessor::Initialize         ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{
    assert( m_avFrames.GetNumEntries() == 0 );    

    for( auto & a : mapping )
    {
        m_avFrames.SetAVFrame( a.first, nullptr );
    }

    m_inputChannels.PreInitialize( uniqueOutputSetups, mapping );
}

} //bv
} //nrl

#include "stdafx.h"

#include "OutputCompositeVideo.h"


// FIXME: make sure that after each frame all VideoInputChannels are invalidated (so that no cached textures are used next frame)

namespace bv { namespace nrl {

// *********************************
//
OutputCompositeVideo::OutputCompositeVideo                          ( unsigned int width, unsigned int height )
    : m_mainWidth( width )
    , m_mainHeight( height )
{
}

// *********************************
//
OutputCompositeVideo::~OutputCompositeVideo                         ()
{
}

// *********************************
//
void    OutputCompositeVideo::ProcessFrameData                      ( NRenderContext *, RenderedChannelsData * input )
{
    m_outputsPreprocessor.InvalidateCachedData  ();

    auto & av = m_outputsPreprocessor.Preprocess( input );

    m_outputsHandler.HandleOutputsData( av );
}

// *********************************
//
OutputCompositeVideo *  OutputCompositeVideo::Create                ( unsigned int width, unsigned int height, OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{

    return nullptr;
}

} //nrl
} //bv


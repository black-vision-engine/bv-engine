#include "stdafx.h"

#include "VideoInputChannelsData.h"


namespace bv { namespace nrl {

// **************************
//
VideoInputChannelsData::VideoInputChannelsData                              ( const RenderedChannelsData * renderedChannelsData )
    : m_wrappedRenderedChannelsData( renderedChannelsData )
{
    
}

// **************************
//
VideoInputChannelsData::~VideoInputChannelsData                             ()
{
}

// **************************
//
const VideoInputChannel *   VideoInputChannelsData::GetInputChannel         ( RenderChannelType rct ) const
{
}

// **************************
//
void                        VideoInputChannelsData::InvalidateCachedTextres ();

// **************************
//
void                        VideoInputChannelsData::InvalidateCachedTexture ( RenderChannelType rct );

// **************************
//
Texture2DPtr                VideoInputChannelsData::ReadColorTexture        ( Renderer * renderer, RenderChannelType rct ) const;

// **************************
//
bool                        VideoInputChannelsData::IsActive                ( RenderChannelType rct ) const;

// **************************
//
bool                        VideoInputChannelsData::ContainsValidData       ( RenderChannelType rct ) const;

// **************************
//
VideoInputChannelsData *    VideoInputChannelsData::Create                  ( const RenderedChannelsData * renderedChannelsData ); 

} //nrl
} //bv

#include "stdafx.h"

#include "VideoInputChannel.h"


namespace bv { namespace nrl {

// **************************
//
VideoInputChannel::VideoInputChannel                            ( const RenderChannel * wrappedChannel )
{
}

// **************************
//
VideoInputChannel::~VideoInputChannel                           ()
{
}

// **************************
//
bool            VideoInputChannel::ContainsValidData            () const
{
}

// **************************
//
void            VideoInputChannel::SetContainsValidData         ( bool isDataValid )
{
}

// **************************
//
Texture2DPtr    VideoInputChannel::ReadColorTexture             ( Renderer * renderer ) const
{
}

// **************************
//
void            VideoInputChannel::InvalidateCachedTexture      () const
{
}

// **************************
//
VideoInputChannel *  VideoInputChannel::Create                  ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height )
{
}

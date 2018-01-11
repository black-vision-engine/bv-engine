#include "stdafx.h"
#include "VideoInputHandler.h"




namespace bv
{

// ***********************
//
VideoInputHandler::VideoInputHandler( videocards::VideoCardManager * videoCardMan, InputSlotsPtr slots )
    :   m_videoCardManager( videoCardMan )
    ,   m_inputSlots( slots )
    ,   m_audioRenderer( nullptr )
    ,   m_renderer( nullptr )
{
    assert( videoCardMan );
    assert( slots );
}

// ***********************
//
VideoInputHandler::~VideoInputHandler()
{
    // Call as not virtual function. Virtuals in destructor not allowed.
    VideoInputHandler::UnregisterInputs();
}

// ***********************
//
void            VideoInputHandler::UnregisterInputs ()
{
    if( m_renderer && m_audioRenderer )
    {
        RenderContext ctx;
        ctx.SetRenderer( m_renderer );
        ctx.SetAudio( m_audioRenderer );

        m_inputSlots.UnregisterAllChannels( &ctx );
    }
}

// ***********************
//
void            VideoInputHandler::ProcessInputs    ( RenderContext * )
{
    auto inputFrameData = m_videoCardManager->QueryVideoInput();
    ProcessFrameData( inputFrameData );
}

// ***********************
//
void            VideoInputHandler::RegisterInputs   ( RenderContext * ctx, InputSlotsPtr inputSlots )
{
    m_renderer = ctx->GetRenderer();
    m_audioRenderer = ctx->GetAudio();

    auto inputDescs = m_videoCardManager->GetInputChannelsDescs();
    RegisterInputs( inputDescs );
}

// ***********************
//
void            VideoInputHandler::RegisterInputs   ( const videocards::InputChannelsDescsVec & channelsDesc )
{
    for( auto & desc : channelsDesc )
    {
        // Don't check result. Error message is logged internally.
        m_inputSlots.RegisterVideoInputChannel( desc );
    }
}

// ***********************
//
void            VideoInputHandler::ProcessFrameData ( videocards::VideoInputFrameData & frameData )
{
    for( auto & singleFrame : frameData.Frames )
    {
        // Note that it's fully legal to get nullptr here. Video input could not
        // provide any frame. In this case input slots remains with previous content.
        if( singleFrame.FrameData )
        {
            m_inputSlots.UpdateVideoInput( singleFrame.InputID, singleFrame.FrameData );
        }
    }
}


}	// bv



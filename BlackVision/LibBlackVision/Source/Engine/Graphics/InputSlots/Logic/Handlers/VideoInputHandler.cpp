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
{}

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
void            VideoInputHandler::ProcessInputs    ( RenderContext * ctx )
{
    ctx;
    assert( false );
}

// ***********************
//
void            VideoInputHandler::RegisterInputs   ( RenderContext * ctx, InputSlotsPtr inputSlots )
{
    m_renderer = ctx->GetRenderer();
    m_audioRenderer = ctx->GetAudio();



    inputSlots;
    assert( false );
}


}	// bv



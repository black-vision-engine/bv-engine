#include "stdafx.h"
#include "VideoInputHandler.h"




namespace bv
{

// ***********************
//
VideoInputHandler::VideoInputHandler( videocards::VideoCardManager * videoCardMan )
    :   m_videoCardManager( videoCardMan )
    ,   m_audioRenderer( nullptr )
    ,   m_renderer( nullptr )
{}

// ***********************
//
void            VideoInputHandler::UnregisterInputs ()
{
    assert( false );
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



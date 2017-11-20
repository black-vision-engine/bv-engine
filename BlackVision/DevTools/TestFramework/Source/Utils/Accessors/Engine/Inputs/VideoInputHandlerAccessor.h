#pragma once


#include "CoreDEF.h"
#include "Engine/Graphics/InputSlots/Logic/Handlers/VideoInputHandler.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"




namespace bv
{


// ***********************
//
class TEST_ACCESSOR( VideoInputHandler )
{
private:
public:

    static  void        RegisterInputs          ( VideoInputHandlerPtr handler, const videocards::InputChannelsDescsVec & channelsDesc );
    static  void        UnregisterInputs        ( VideoInputHandlerPtr handler );

    static void         SetRenderContext        ( VideoInputHandlerPtr handler, Renderer * renderer, audio::AudioRenderer * audio );
};


// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline void         TEST_ACCESSOR( VideoInputHandler )::RegisterInputs      ( VideoInputHandlerPtr handler, const videocards::InputChannelsDescsVec & channelsDesc )
{
    handler->RegisterInputs( channelsDesc );
}

// ***********************
//
inline void         TEST_ACCESSOR( VideoInputHandler )::UnregisterInputs      ( VideoInputHandlerPtr handler )
{
    handler->UnregisterInputs();
}

// ***********************
//
inline void         TEST_ACCESSOR( VideoInputHandler )::SetRenderContext       ( VideoInputHandlerPtr handler, Renderer * renderer, audio::AudioRenderer * audio )
{
    handler->m_renderer = renderer;
    handler->m_audioRenderer = audio;
}


}   // bv


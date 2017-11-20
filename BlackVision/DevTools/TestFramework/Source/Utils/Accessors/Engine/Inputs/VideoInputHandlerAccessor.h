#pragma once


#include "CoreDEF.h"
#include "Engine/Graphics/InputSlots/Logic/Handlers/VideoInputHandler.h"



namespace bv
{


// ***********************
//
class TEST_ACCESSOR( VideoInputHandler )
{
private:
public:

   static  void        RegisterInputs          ( VideoInputHandlerPtr handler, const videocards::InputChannelsDescsVec & channelsDesc );

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



}   // bv


#pragma once


#include "InputSlotDescriptor.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Audio/Resources/AudioBuffer.h"

#include <string>


namespace bv
{


// ***********************
///@ingroup EngineInputs
struct InputSlot
{
    Texture2DPtr            Texture;
    audio::AudioBufferPtr   Audio;

// ***********************
//
    InputSlot()
        : Audio( nullptr )
        , Texture( nullptr )
    {}

// ***********************
//
    InputSlot( Texture2DPtr tex )
        : Audio( nullptr )
        , Texture( tex )
    {}

// ***********************
//
    InputSlot( Texture2DPtr tex, audio::AudioBufferPtr audio )
        : Audio( audio )
        , Texture( tex )
    {}

// ***********************
//
    InputSlot( audio::AudioBufferPtr audio )
        : Audio( audio )
        , Texture( nullptr )
    {}
};

// ***********************
/// @ingroup EngineInputs
struct InputEntry
{
    InputSlot               Slot;
    InputSlotDescriptor     Descriptor;

    Int32                   References;

// ***********************
//
    InputEntry()
        : References( 0 )
    {}

// ***********************
//
    InputEntry( const InputSlot & slot )
        : References( 0 )
        , Slot( slot )
    {}
};




}	// bv



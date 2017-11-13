#pragma once


#include "InputSlotDescriptor.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Audio/AudioEntity.h"

#include <string>


namespace bv
{


// ***********************
//
struct InputSlot
{
    Texture2DPtr            Texture;
    audio::AudioEntity *    Audio;

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
    InputSlot( Texture2DPtr tex, audio::AudioEntity * audio )
        : Audio( audio )
        , Texture( tex )
    {}

// ***********************
//
    InputSlot( audio::AudioEntity * audio )
        : Audio( audio )
        , Texture( nullptr )
    {}
};

// ***********************
//
struct InputEntry
{
    InputSlot               Slot;
    InputSlotDescriptor     Descriptor;

    UInt32                  References;

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



#pragma once


#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include <string>


namespace bv
{


// ***********************
//
struct TextureSlot
{
    Texture2DPtr        Texture;

    std::string         SlotName;
    UInt64              References;

// ***********************
//
    TextureSlot()
        : References( 0 )
    {}

};


}	// bv



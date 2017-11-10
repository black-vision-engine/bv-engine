#pragma once


#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include <string>


namespace bv
{


// ***********************
//
struct InputSlot
{
    Texture2DPtr        Texture;

    std::string         SlotName;
    UInt64              References;

// ***********************
//
    InputSlot()
        : References( 0 )
    {}

};


}	// bv



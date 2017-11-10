#pragma once

#include "Engine/Graphics/InputSlots/InputSlots.h"



namespace bv
{


class Texture2DCache;


// ***********************
//
struct EngineResources
{
    Texture2DCache *    TexturesMap;
    InputSlotsPtr       InputSlots;

    // ***********************
    //
    EngineResources()
        : TexturesMap( nullptr )
        , InputSlots( nullptr )
    {}
};


}	// bv



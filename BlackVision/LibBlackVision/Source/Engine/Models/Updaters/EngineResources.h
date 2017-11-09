#pragma once

#include "Engine/Graphics/InputSlots/TextureSlots.h"



namespace bv
{


class Texture2DCache;


// ***********************
//
struct EngineResources
{
    Texture2DCache *    TexturesMap;
    TextureSlotsPtr     InputSlots;

    // ***********************
    //
    EngineResources()
        : TexturesMap( nullptr )
        , InputSlots( nullptr )
    {}
};


}	// bv



#pragma once

#include "Engine/Graphics/Resources/Textures/Texture2D.h"



namespace bv
{



// ========================================================================= //
// Helpers
// ========================================================================= //

// ***********************
//
inline
Texture2DPtr        CreateFakeTexture   ( UInt32 width, UInt32 height )
{
    return std::make_shared< Texture2D >( TextureFormat::F_A8R8G8B8, width, height, DataBuffer::Semantic::S_TEXTURE_DYNAMIC, 1 );
}




}	// bv




#pragma once

#include "Engine/Types/Enums.h"


namespace bv {

struct RenderTargetDataEntryDesc
{
    RenderTargetDataEntryDesc( unsigned int width_, unsigned int height_, TextureFormat fmt_ );

    unsigned int    width;
    unsigned int    height;

    TextureFormat   fmt;
};

} //bv

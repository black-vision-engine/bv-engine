#pragma once

#include "Engine/Interfaces/IValue.h"

namespace bv
{

class FullscreenEffect;

class ImageMaskFSECreator
{
public: 
    static FullscreenEffect *       CreateImageMaskFSE      ( const std::vector< IValuePtr > & values );
};

} // bv
#pragma once

#include "Engine/Interfaces/IValue.h"

namespace bv
{

class FullscreenEffect;

class BlurFSECreator
{
public: 
    static FullscreenEffect *       CreateSimpleFSE         ( const std::vector< IValuePtr > & values );
    static FullscreenEffect *       CreateCompositeFSE      ( const std::vector< IValuePtr > & values );
    static FullscreenEffect *       CreateCompositeOneFSE   ( const std::vector< IValuePtr > & values );
};

} // bv
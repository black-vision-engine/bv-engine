#pragma once

#include "Engine/Interfaces/IValue.h"

namespace bv
{

class SimpleFullscreenEffect;
class CompositeFullscreenEffect;

class BlurFSECreator
{
public: 
    static SimpleFullscreenEffect *     CreateSimpleFSE         ( const std::vector< IValuePtr > & values );
    static CompositeFullscreenEffect *  CreateCompositeFSE      ( const std::vector< IValuePtr > & values );
    static CompositeFullscreenEffect *  CreateCompositeOneFSE   ( const std::vector< IValuePtr > & values );
};

} // bv
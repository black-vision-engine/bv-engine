#pragma once

#include "Engine/Interfaces/IValue.h"

namespace bv
{

class FullscreenEffect;

class ShadowFSECreator
{
public: 
    static FullscreenEffect *       CreateShadowFSE         ( const std::vector< IValuePtr > & values );
};

} // bv
#pragma once

#include "Engine/Interfaces/IValue.h"

namespace bv
{

class FullscreenEffect;

class LightScatteringMaskFSECreator
{
public: 
    static FullscreenEffect *       CreateLightScatteringMaskFSE      ( const std::vector< IValuePtr > & values );
};

} // bv
#pragma once

#include "Engine/Interfaces/IValue.h"

namespace bv
{

class FullscreenEffect;

class LightScatteringFSECreator
{
public: 
    static FullscreenEffect *       CreateLightScatteringFSE      ( const std::vector< IValuePtr > & values );
};

} // bv
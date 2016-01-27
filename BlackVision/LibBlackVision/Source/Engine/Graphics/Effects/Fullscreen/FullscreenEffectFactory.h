#pragma once

#include <vector>

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffectTypes.h"
#include "Engine/Interfaces/IValue.h"


namespace bv {

class FullscreenEffect;

FullscreenEffect *  CreateFullscreenEffect  ( FullscreenEffectType fseType, const std::vector< IValuePtr > & values );
FullscreenEffect *  CreateFullscreenEffect  ( FullscreenEffectType fseType );

} // bv

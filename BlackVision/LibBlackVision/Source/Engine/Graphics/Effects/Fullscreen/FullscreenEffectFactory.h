#pragma once

#include <vector>

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffect.h"
#include "Engine/Graphics/Effects/FullScreen/FullscreenEffectInstance.h"



namespace bv {

class FullscreenEffect;

FullscreenEffect *  CreateFullscreenEffect                  ( FullscreenEffectType fseType, const std::vector< IValuePtr > & values );
FullscreenEffect *  CreateFullscreenEffect                  ( FullscreenEffectType fseType );

FullscreenEffectInstance *  CreateFullscreenEffectInstance  ( FullscreenEffectType fseType, const std::vector< IValuePtr > & values );
FullscreenEffectInstance *  CreateFullscreenEffectInstance  ( FullscreenEffectType fseType );

} // bv

#pragma once

#include <vector>

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffectTypes.h"
#include "Engine/Interfaces/IValue.h"


namespace bv {

class FullscreenEffectTr;

FullscreenEffectTr *    CreateFullscreenEffect( FullscreenEffectType fseType, const std::vector< IValuePtr > & values );
FullscreenEffectTr *    CreateFullscreenEffect( FullscreenEffectType fseType );

} // bv

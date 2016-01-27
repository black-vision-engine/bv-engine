#pragma once

#include "Engine/Graphics/Effects/NodeEffect/NodeEffectTypes.h"
#include "Engine/Graphics/Effects/NodeEffect/NodeEffect.h"


namespace bv {

class NodeEffect;

NodeEffectPtr   CreateNodeEffect( NodeEffectType nodeEffectType );

} //bv

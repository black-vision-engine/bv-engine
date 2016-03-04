#include "stdafx.h"

#include "ShadowFSE.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/SimpleFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"
#include "Engine/Graphics/Effects/Fullscreen/Impl/Graph/FullscreenEffectGraph.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"

namespace bv
{

// **************************
//
FullscreenEffect *       ShadowFSECreator::CreateShadowFSE         ( const std::vector< IValuePtr > & values )
{
    { values; }
    return nullptr;
}

} // bv
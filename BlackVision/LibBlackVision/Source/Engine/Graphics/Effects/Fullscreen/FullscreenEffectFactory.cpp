#include "FullscreenEffectFactory.h"

#include <cassert>


namespace bv {

// **************************
//
FullscreenEffectTr *    CreateFullscreenEffect( FullscreenEffectTypes fseType )
{
    switch( fseType )
    {
        case FullscreenEffectTypes::FET_SIMPLE_BLIT:
            return nullptr;
        case FullscreenEffectTypes::FET_BLIT_WITH_ALPHA:
            return nullptr;
        case FullscreenEffectTypes::FET_BLIT_WITH_ALPHA_MASK:
            return nullptr;
        case FullscreenEffectTypes::FET_INTERLACE:
            return nullptr;
        default:
            assert( false );
    }

    return nullptr;
}

} // bv

#include "FullscreenEffectFactory.h"

#include <cassert>


namespace bv {

// **************************
//
FullscreenEffectTr *    CreateSimpleBlitFSE         ()
{
    return nullptr;
}

// **************************
//
FullscreenEffectTr *    CreateBlitWithAlphaFSE      ()
{
    return nullptr;
}

// **************************
//
FullscreenEffectTr *    CreateBlitWithAlphaMaskFSE  ()
{
    return nullptr;
}

// **************************
//
FullscreenEffectTr *    CreateInterlaceFSE          ()
{
    return nullptr;
}

namespace {
}

// **************************
//
FullscreenEffectTr *    CreateFullscreenEffect( FullscreenEffectTypes fseType )
{
    switch( fseType )
    {
        case FullscreenEffectTypes::FET_SIMPLE_BLIT:
            return CreateSimpleBlitFSE();
        case FullscreenEffectTypes::FET_BLIT_WITH_ALPHA:
            return CreateBlitWithAlphaFSE();
        case FullscreenEffectTypes::FET_BLIT_WITH_ALPHA_MASK:
            return CreateBlitWithAlphaMaskFSE();
        case FullscreenEffectTypes::FET_INTERLACE:
            return CreateInterlaceFSE();
        default:
            assert( false );
    }

    return nullptr;
}

} // bv

#pragma once


namespace bv {

enum class FullscreenEffectType : int
{
    FET_SIMPLE_BLIT = 0,
    FET_BLIT_WITH_ALPHA,
    FET_BLIT_WITH_ALPHA_MASK,
    FET_INTERLACE,

    //FET_BLUR
    //FET_SHADOW
    //FET_OUTLINE
    //TODO: and so on

    FET_TOTAL
};

} // bv

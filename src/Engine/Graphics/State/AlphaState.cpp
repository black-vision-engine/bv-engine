#include "AlphaState.h"


namespace bv {

// *******************************
//
AlphaState::AlphaState      ()
    : blendEnabled  ( false )
    , srcBlendMode  ( AlphaSrcBlendMode::ASBM_SRC_ALPHA )
    , dstBlendMode  ( AlphaDstBlendMode::ADBM_ONE_MINUS_SRC_ALPHA )
    , compareEnabled( false )
    , compareMode   ( AlphaCompareMode::ACM_ALWAYS )
    , alphaReference( 0.f )
    , blendColor    ( glm::vec4( 0.f, 0.f, 0.f, 0.f ) )    
{
}

}

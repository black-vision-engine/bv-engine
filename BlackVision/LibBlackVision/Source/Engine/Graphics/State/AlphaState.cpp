#include "AlphaState.h"


namespace bv {

// *******************************
//
AlphaState::AlphaState      ()
    : blendEnabled  ( false )
    , srcBlendMode  ( AlphaSrcBlendMode::ASBM_ONE )
    , dstBlendMode  ( AlphaDstBlendMode::ADBM_ONE_MINUS_SRC_ALPHA )
    , blendColor    ( glm::vec4( 0.f, 0.f, 0.f, 0.f ) )    
{
}

}

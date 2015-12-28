#include "AlphaState.h"


namespace bv {

// *******************************
//
AlphaState::AlphaState      ()
    : blendEnabled  ( false )
    , srcRGBBlendMode  ( AlphaSrcBlendMode::ASBM_ONE )
    , dstRGBBlendMode  ( AlphaDstBlendMode::ADBM_ONE_MINUS_SRC_ALPHA )
    , srcAlphaBlendMode  ( AlphaSrcBlendMode::ASBM_ONE )
	, dstAlphaBlendMode  ( AlphaDstBlendMode::ADBM_ONE )
    , blendColor    ( glm::vec4( 0.f, 0.f, 0.f, 0.f ) )    
{
}

}

#include "DepthState.h"


namespace bv {

// *******************************
//
DepthState::DepthState  ()
    : enabled       ( true )
    , writable      ( true )
    , compareMode   ( DepthCompareMode::DCM_LEQUAL )
{
}

} //bv

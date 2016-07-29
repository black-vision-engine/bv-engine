#include "stdafx.h"

#include "DepthState.h"




#include "Memory/MemoryLeaks.h"



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

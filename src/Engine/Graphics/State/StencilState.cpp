#include "StencilState.h"

namespace bv {

// *******************************
//
StencilState::StencilState  ()
    : enabled       ( false )    
    , compareMode   ( StencilCompareMode::SCM_NEVER )
    , reference     ( 0 )  
    , andMask       ( 0xFFFFFFFF )    
    , writeMask     ( 0xFFFFFFFF )  
    , actionFail    ( StencilOperationType::SOT_KEEP )
    , actionZFail   ( StencilOperationType::SOT_KEEP )
    , actionZPass   ( StencilOperationType::SOT_KEEP )
{
}

} //bv

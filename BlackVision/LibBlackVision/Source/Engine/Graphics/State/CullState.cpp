#include "stdafx.h"

#include "CullState.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *******************************
//
CullState::CullState  ()
    : enabled       ( true )
    , isCCWOrdered  ( true )
{
}

} //bv

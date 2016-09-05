#include "stdafx.h"

#include "OffsetState.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *******************************
// FIXME: rethink this state
OffsetState::OffsetState()
    : fillEnabled   ( false )
    , linesEnabled  ( false )
    , pointsEnabled ( false )
    , scale         ( 0.f )
    , bias          ( 0.f )
{
}

}

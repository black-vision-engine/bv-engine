#include "stdafxCore.h"

#include "InitSubsystem.h"

#include <cassert>


namespace bv {

int InitSubsystem::m_sNumInitializers = 0;
InitSubsystem::Initializer InitSubsystem::m_sInitializers[ MAX_ELEMENTS ];

// *********************************
// FIXME: thread safety
void InitSubsystem::AddInitializer ( Initializer fun )
{
    if ( m_sNumInitializers < MAX_ELEMENTS )
    {
        m_sInitializers[ m_sNumInitializers++ ] = fun;
    }
    else
    {
        assert( false );
    }
}

// *********************************
// FIXME: thread safety
void InitSubsystem::CallInitializers	( int argc, char * argv[] )
{
    for ( int i = 0; i < m_sNumInitializers; ++i )
    {
        m_sInitializers[i]( argc, argv );
    }
}

} // bv

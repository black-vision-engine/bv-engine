#include "RenderableEffect.h"
#include <cassert>

namespace bv {

// *********************************
//
RenderableEffect::RenderableEffect    ()
{
}

// *********************************
//
RenderableEffect::~RenderableEffect   ()
{
}

// *********************************
//
int                 RenderableEffect::NumPasses             () const
{
    return m_passes.size();
}

// *********************************
//
void                RenderableEffect::AddPass               ( RenderablePass * pass )
{
    m_passes.push_back( pass );
}

// *********************************
//
RenderablePass *  RenderableEffect::GetPass                 ( int index )
{
    assert( index >= 0 );
    assert( index < NumPasses() );

    return m_passes[ index ];
}

}

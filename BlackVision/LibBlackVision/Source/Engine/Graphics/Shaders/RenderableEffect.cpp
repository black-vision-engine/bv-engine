#include "RenderableEffect.h"

#include <cassert>


namespace bv {

// *********************************
//
RenderableEffect::RenderableEffect              ()
{
}

// *********************************
//
RenderableEffect::~RenderableEffect             ()
{
    for( auto pass : m_passes )
    {
        delete pass;
    }
}

// *********************************
//
void                RenderableEffect::AddPass   ( RenderablePass * pass )
{
    m_passes.push_back( pass );
}

} //bv

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
}

// *********************************
//
void                RenderableEffect::AddPass   ( RenderablePass * pass )
{
    m_passes.push_back( pass );
}

}

#include "stdafx.h"

#include "RenderableEffect.h"

#include <cassert>




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
RenderableEffect::RenderableEffect              ()
{
}

// ***********************
//
RenderableEffect::RenderableEffect  ( RenderablePass * pass )
{
    m_passes.push_back( pass );
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

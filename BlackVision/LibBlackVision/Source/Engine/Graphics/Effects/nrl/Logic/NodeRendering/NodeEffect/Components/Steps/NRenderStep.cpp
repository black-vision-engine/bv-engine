#include "stdafx.h"

#include "NRenderStep.h"


namespace bv { namespace nrl {

// **************************
//
NRenderStep::NRenderStep                                    ( NRenderComponentStatePtr initialState )
    : m_state( initialState )
{
}

// **************************
//
NRenderComponentStatePtr        NRenderStep::GetState       ()
{
    return m_state;
}

// **************************
//
NRenderComponentStateConstPtr  NRenderStep::GetState        () const
{
    return m_state;
}

// **************************
//
void                            NRenderStep::ReadInputState ()
{
    // Intentionally lefty empty
}

// **************************
//
void                            NRenderStep::SetState       ( NRenderComponentStatePtr state )
{
    m_state = state;
}

// **************************
//
bool                            NRenderStep::IsIdle         ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return false;
}

// **************************
//
bool                            NRenderStep::IsFinal        ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return false;
}

// **************************
//
void                            NRenderStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    { passes; }
}

} // nrl
} // bv

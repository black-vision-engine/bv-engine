#include "stdafx.h"

#include "RenderStep.h"


namespace bv { namespace nrl {

// **************************
//
RenderStep::RenderStep                                    ( RenderComponentStatePtr initialState )
    : m_state( initialState )
{
}

// **************************
//
RenderComponentStatePtr        RenderStep::GetState       ()
{
    return m_state;
}

// **************************
//
RenderComponentStateConstPtr  RenderStep::GetState        () const
{
    return m_state;
}

// **************************
//
void                            RenderStep::ReadInputState ()
{
    // Intentionally lefty empty
}

// **************************
//
void                            RenderStep::SetState       ( RenderComponentStatePtr state )
{
    m_state = state;
}

// **************************
//
bool                            RenderStep::IsIdle         ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return false;
}

// **************************
//
bool                            RenderStep::IsFinal        ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return false;
}

// **************************
//
void                            RenderStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    { passes; }
}

} // nrl
} // bv

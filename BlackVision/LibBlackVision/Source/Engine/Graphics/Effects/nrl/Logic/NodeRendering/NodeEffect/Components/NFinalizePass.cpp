#include "stdafx.h"

#include "NFinalizePass.h"


namespace bv { namespace nrl {

// **************************
//
NFinalizePass::NFinalizePass                ( NFinalizeStep * finalizeStep )
    : m_finalizeStep( finalizeStep )
{
}

// **************************
//
NFinalizePass::~NFinalizePass               ()
{
    delete m_finalizeStep;
}

// **************************
//
void        NFinalizePass::ReadInputState   ()
{
    m_finalizeStep->ReadInputState();
}

// **************************
//
void        NFinalizePass::Apply            ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    m_finalizeStep->Apply( nodeRepr, ctx );

}

// **************************
//
bool        NFinalizePass::IsFinal          ( SceneNodeRepr * nodeRepr ) const
{
    return m_finalizeStep->IsFinal( nodeRepr );
}

// **************************
//
bool        NFinalizePass::IsIdle           ( SceneNodeRepr * nodeRepr ) const
{
    return m_finalizeStep->IsIdle( nodeRepr );
}

// **************************
//
IValuePtr   NFinalizePass::GetValue         ( const std::string & name ) const
{
    IValuePtr res = nullptr;

    if( m_finalizeStep )
    {
        res = m_finalizeStep->GetState()->GetValue( name );
    }

    return res;
}

// **************************
//
void        NFinalizePass::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_finalizeStep->AppendRenderPasses_DIRTY_HACK( passes );
}

} // nrl
} // bv

#include "stdafx.h"

#include "FinalizePass.h"


namespace bv { 

// **************************
//
FinalizePass::FinalizePass                ( FinalizeStep * finalizeStep )
    : m_finalizeStep( finalizeStep )
{
}

// **************************
//
FinalizePass::~FinalizePass               ()
{
    delete m_finalizeStep;
}

// **************************
//
void        FinalizePass::ReadInputState   ()
{
    m_finalizeStep->ReadInputState();
}

// **************************
//
void        FinalizePass::Apply            ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    m_finalizeStep->Apply( nodeRepr, ctx );

}

// **************************
//
bool        FinalizePass::IsFinal          ( SceneNodeRepr * nodeRepr ) const
{
    return m_finalizeStep->IsFinal( nodeRepr );
}

// **************************
//
bool        FinalizePass::IsIdle           ( SceneNodeRepr * nodeRepr ) const
{
    return m_finalizeStep->IsIdle( nodeRepr );
}

// **************************
//
IValuePtr   FinalizePass::GetValue         ( const std::string & name ) const
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
void        FinalizePass::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_finalizeStep->AppendRenderPasses_DIRTY_HACK( passes );
}


} // bv

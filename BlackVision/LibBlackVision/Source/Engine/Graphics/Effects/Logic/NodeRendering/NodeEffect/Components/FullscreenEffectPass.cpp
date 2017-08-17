#include "stdafx.h"

#include "FullscreenEffectPass.h"


namespace bv { 

// **************************
//
FullscreenEffectPass::FullscreenEffectPass            ( PreFullscreenEffectStep * preFullscreenEffectStep, FullscreenEffectStep * fullscreenEffectStep )
    : m_preFullscreenStep( preFullscreenEffectStep )
    , m_fullscreenStep( fullscreenEffectStep )
{
    assert( preFullscreenEffectStep->GetNumOutputs() == fullscreenEffectStep->GetNumRequiredInputs() );
}

// **************************
//
FullscreenEffectPass::~FullscreenEffectPass           ()
{
    delete m_preFullscreenStep;
    delete m_fullscreenStep;
}

// **************************
//
void    FullscreenEffectPass::ReadInputState           ()
{
    m_preFullscreenStep->ReadInputState();
    m_fullscreenStep->ReadInputState();
}

// **************************
// It ia assumed that ReadInputState has already been called
void    FullscreenEffectPass::Apply                    ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    assert( !( m_preFullscreenStep->IsIdle( nodeRepr ) && m_fullscreenStep->IsIdle( nodeRepr ) ) );

    const RenderedData * res = nullptr;

    if( !m_preFullscreenStep->IsIdle( nodeRepr ) )
    {
        res = m_preFullscreenStep->Apply( nodeRepr, ctx );;
    }
   
    if( !m_preFullscreenStep->IsFinal( nodeRepr ) && !m_fullscreenStep->IsIdle( nodeRepr ) )
    {
        m_fullscreenStep->Apply( ctx, res );
    }
}

// **************************
//
bool    FullscreenEffectPass::IsFinal                  ( SceneNodeRepr * nodeRepr ) const
{
    return m_preFullscreenStep->IsFinal( nodeRepr ) || m_fullscreenStep->IsFinal( nodeRepr );
}

// **************************
//
bool    FullscreenEffectPass::IsIdle                   ( SceneNodeRepr * nodeRepr ) const
{
    return m_preFullscreenStep->IsIdle( nodeRepr ) && m_fullscreenStep->IsIdle( nodeRepr );
}

// **************************
// FIXME: remove and refactror updates logic and implementation
IValuePtr   FullscreenEffectPass::GetValue             ( const std::string & name ) const
{
    IValuePtr res = m_preFullscreenStep->GetState()->GetValue( name );

    if ( !res &&  m_fullscreenStep->GetState() )
    {
        res = m_fullscreenStep->GetState()->GetValue( name );
    }

    return res;
}

// **************************
//
void        FullscreenEffectPass::AppendRenderPasses_DIRTY_HACK    ( std::set< const RenderablePass * > * passes ) const
{
    m_preFullscreenStep->AppendRenderPasses_DIRTY_HACK( passes );
    m_fullscreenStep->AppendRenderPasses_DIRTY_HACK( passes );
}


} // bv

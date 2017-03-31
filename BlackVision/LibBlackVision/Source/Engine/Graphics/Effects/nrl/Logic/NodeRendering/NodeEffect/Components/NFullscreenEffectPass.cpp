#include "stdafx.h"

#include "NFullscreenEffectPass.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffectPass::NFullscreenEffectPass            ( NPreFullscreenEffectStep * preFullscreenEffectStep, NFullscreenEffectStep * fullscreenEffectStep )
    : m_preFullscreenStep( preFullscreenEffectStep )
    , m_fullscreenStep( fullscreenEffectStep )
{
    assert( preFullscreenEffectStep->GetNumOutputs() == fullscreenEffectStep->GetNumRequiredInputs() );
}

// **************************
//
NFullscreenEffectPass::~NFullscreenEffectPass           ()
{
    delete m_preFullscreenStep;
    delete m_fullscreenStep;
}

// **************************
//
void    NFullscreenEffectPass::ReadInputState           ()
{
    m_preFullscreenStep->ReadInputState();
    m_fullscreenStep->ReadInputState();
}

// **************************
// It ia assumed that ReadInputState has already been called
void    NFullscreenEffectPass::Apply                    ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    assert( !( m_preFullscreenStep->IsIdle( nodeRepr ) && m_fullscreenStep->IsIdle( nodeRepr ) ) );

    const NRenderedData * res = nullptr;

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
bool    NFullscreenEffectPass::IsFinal                  ( SceneNodeRepr * nodeRepr ) const
{
    return m_preFullscreenStep->IsFinal( nodeRepr ) || m_fullscreenStep->IsFinal( nodeRepr );
}

// **************************
//
bool    NFullscreenEffectPass::IsIdle                   ( SceneNodeRepr * nodeRepr ) const
{
    return m_preFullscreenStep->IsIdle( nodeRepr ) && m_fullscreenStep->IsIdle( nodeRepr );
}

// **************************
// FIXME: remove and refactror updates logic and implementation
IValuePtr   NFullscreenEffectPass::GetValue             ( const std::string & name ) const
{
    IValuePtr res = m_preFullscreenStep->GetState()->GetValue( name );

    if ( !res )
    {
        res = m_fullscreenStep->GetState()->GetValue( name );
    }

    return res;
}

// **************************
//
void        NFullscreenEffectPass::AppendRenderPasses_DIRTY_HACK    ( std::set< const RenderablePass * > * passes ) const
{
    m_preFullscreenStep->AppendRenderPasses_DIRTY_HACK( passes );
    m_fullscreenStep->AppendRenderPasses_DIRTY_HACK( passes );
}

} // nrl
} // bv

#include "stdafx.h"

#include "LightScatteringFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
LightScatteringFSEStep::LightScatteringFSEStep          ()
    : Parent( nullptr )

{
    // FIXME: maybe one class is enough as we only use FullscreenEffectType type here and always set FSE state as current state
    m_lsEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_LIGHT_SCATTERING );
    SetState( m_lsEffect->GetState() );
}

// **************************
//
LightScatteringFSEStep::~LightScatteringFSEStep           ()
{
    delete m_lsEffect;
}

// **************************
//
unsigned int            LightScatteringFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    LightScatteringFSEStep::ReadInputState               ()
{}

// **************************
//
void                    LightScatteringFSEStep::ApplyImpl                    ( RenderContext * ctx, const RenderedData * input )
{
    assert( input );

	m_lsEffect->Render( ctx, *input );
}

// **************************
//
void                    LightScatteringFSEStep::FreeRenderTargets            ( RenderContext * ctx, const RenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    LightScatteringFSEStep::IsIdle                       ( SceneNodeRepr * ) const
{
	return false;
}

// **************************
// If 
bool                    LightScatteringFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	return true;
}

// **************************
//
void                LightScatteringFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_lsEffect->AppendRenderPasses_DIRTY_HACK( passes );
}

} // nrl
} // bv

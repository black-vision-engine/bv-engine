#include "stdafx.h"

#include "NLightScatteringFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NLightScatteringFSEStep::NLightScatteringFSEStep          ()
    : Parent( nullptr )

{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_lsEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_LIGHT_SCATTERING );
    SetState( m_lsEffect->GetState() );
}

// **************************
//
NLightScatteringFSEStep::~NLightScatteringFSEStep           ()
{
    delete m_lsEffect;
}

// **************************
//
unsigned int            NLightScatteringFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NLightScatteringFSEStep::ReadInputState               ()
{}

// **************************
//
void                    NLightScatteringFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
    assert( input );

	m_lsEffect->Render( ctx, *input );
}

// **************************
//
void                    NLightScatteringFSEStep::FreeRenderTargets            ( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    NLightScatteringFSEStep::IsIdle                       ( SceneNodeRepr * ) const
{
	return false;
}

// **************************
// If 
bool                    NLightScatteringFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	return true;
}

// **************************
//
void                NLightScatteringFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_lsEffect->AppendRenderPasses_DIRTY_HACK( passes );
}

} // nrl
} // bv

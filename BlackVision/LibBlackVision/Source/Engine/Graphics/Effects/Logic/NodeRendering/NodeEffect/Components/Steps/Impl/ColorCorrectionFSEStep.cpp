#include "stdafx.h"

#include "ColorCorrectionFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { 

// **************************
//
ColorCorrectionFSEStep::ColorCorrectionFSEStep          ( FullscreenEffectType nnodeEffectType )
    : Parent( nullptr )
    , m_colorCorrectionEffect( nullptr )
    //, m_minThreshold( minAlphaThreshold )
    , m_alpha( 0.f )

{
    // FIXME: maybe one class is enough as we only use FullscreenEffectType type here and always set FSE state as current state
    m_colorCorrectionEffect = CreateFullscreenEffect( nnodeEffectType );

    Parent::SetState( m_colorCorrectionEffect->GetState() );
}

// **************************
//
ColorCorrectionFSEStep::~ColorCorrectionFSEStep           ()
{
    delete m_colorCorrectionEffect;
}

// **************************
//
unsigned int            ColorCorrectionFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    ColorCorrectionFSEStep::ApplyImpl                    ( RenderContext * ctx, const RenderedData * input )
{
    //    assert( input ); // commented out due to FullscreenEffectPass::Apply behaviour, perhaphs one of those should be fixed
    if( input == nullptr )
        return;

    // FIXME: add sync here on smewhere else, where it fits
    m_colorCorrectionEffect->Render( ctx, *input );
}

// **************************
//
void                    ColorCorrectionFSEStep::FreeRenderTargets            ( RenderContext * ctx, const RenderedData * input )
{
    input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    ColorCorrectionFSEStep::IsIdle                       ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }
    return false;
}

// **************************
// If 
bool                    ColorCorrectionFSEStep::IsFinal                      ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }
    return false;
}

// **************************
//
void                ColorCorrectionFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_colorCorrectionEffect->AppendRenderPasses_DIRTY_HACK( passes );
}


} // bv

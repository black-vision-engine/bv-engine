#include "stdafx.h"

#include "NBlurFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NBlurFSEStep::NBlurFSEStep          ()
    : Parent( nullptr )
	, m_resizedRT( nullptr )
	, m_resizedHorizontallyBluredRT( nullptr )
{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blurEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLUR );
	m_simpleBlitEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_SIMPLE_BLIT );
    SetState( m_blurEffect->GetState() );
}

// **************************
//
NBlurFSEStep::~NBlurFSEStep           ()
{
    delete m_blurEffect;
}

// **************************
//
unsigned int            NBlurFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NBlurFSEStep::ReadInputState               ()
{}

// **************************
//
void                    NBlurFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
    assert( input );

	// FIXME: Some general mechanism should be implemented to set values in effect.
	auto textureSize = GetState()->GetValueAt( 0 );

	auto wrt = input->GetEntry( 0 )->Width();
	auto hrt = input->GetEntry( 0 )->Height();

	auto blurQualityLevelVal = GetState()->GetValueAt( 4 );
	auto blurQualityLevel = QueryTypedValue< ValueIntPtr >( blurQualityLevelVal );

	auto blurSizeVal = GetState()->GetValueAt( 1 );
	auto blurSize = QueryTypedValue< ValueFloatPtr >( blurSizeVal )->GetValue();

	// Set textureSize param value needed by blur shader.
	QueryTypedValue< ValueVec2Ptr >( textureSize )->SetValue( glm::vec2( wrt, hrt ) );

	auto mainRT = disableBoundRT( ctx );

	auto stepsNum = blurQualityLevel->GetValue() + 1;

	if( stepsNum > 0 )
	{
		NRenderedData rd( 1 );
		rd.SetEntry( 0, input->GetEntry( 0 ) );

		const RenderTarget * output = nullptr;

		blurSize /= stepsNum;

		for( auto i = 0; i < stepsNum - 1; ++i )
		{
			output = FastBlur( ctx, rd, blurSize, output );
			auto oldInputRT = rd.GetEntry( 0 );
			rd.SetEntry( 0, output );
			output = oldInputRT;

			// Clear blur output render target.
			enable( ctx, output );
			clearBoundRT( ctx, glm::vec4() );
			disableBoundRT( ctx );
		}

		FastBlur( ctx, rd, blurSize, mainRT );

		if( stepsNum > 1 )
		{
			allocator( ctx )->Free();
		}
	}
}

// **************************
//
const RenderTarget *	NBlurFSEStep::FastBlur						( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output )
{
	NRenderedData inputResized( 1 );
	inputResized.SetEntry( 0, input.GetEntry( 0 ) );

	if( blurSize > 16 )
	{
		auto resizedRT = ResizeInput( ctx, input, 0.5f );
		inputResized.SetEntry( 0, resizedRT );
		blurSize *= 0.5f;
	}

	return BlurInput( ctx, inputResized, blurSize, output );
}

// **************************
//
const RenderTarget *	NBlurFSEStep::BlurInput						( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output )
{
	if( !output )
	{
		// Allocate output render target if not passed
		output = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
		NRenderedData rd( 1 );
		rd.SetEntry( 0, output );
		enable( ctx, output );
		clearBoundRT( ctx, glm::vec4() );
		disableBoundRT( ctx );
	}

	auto blurSizeVal = GetState()->GetValueAt( 1 );
	QueryTypedValue< ValueFloatPtr >( blurSizeVal )->SetValue( blurSize );

	auto vertical = GetState()->GetValueAt( 3 );

	// Allocate new render target for vertical blur pass
	//auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
	NRenderedData rd( 1 );
	rd.SetEntry( 0, m_resizedHorizontallyBluredRT );

	// Clear blur output render target.
	enable( ctx, m_resizedHorizontallyBluredRT );
	clearBoundRT( ctx, glm::vec4() );
	disableBoundRT( ctx );

	auto textureSize = GetState()->GetValueAt( 0 );
	auto wrt = input.GetEntry( 0 )->Width();
	auto hrt = input.GetEntry( 0 )->Height();
	QueryTypedValue< ValueVec2Ptr >( textureSize )->SetValue( glm::vec2( wrt, hrt ) );

	// Run vertical blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( true );
	m_blurEffect->Render( ctx, m_resizedHorizontallyBluredRT, input );

	// Run horizontal blur pass
	enable( ctx, output );
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( false );
	m_blurEffect->Render( ctx, rd );

	//allocator( ctx )->Free();  // free allocated locally render target.

	return output;
}

// **************************
//
const RenderTarget *	NBlurFSEStep::ResizeInput					( NRenderContext * ctx, const NRenderedData & input, Float32 scale )
{
	auto inputW = input.GetEntry( 0 )->Width();
	auto inputH = input.GetEntry( 0 )->Height();

	auto outputW = UInt32( std::ceil( inputW * scale ) );
	auto outputH = UInt32( std::ceil( inputH * scale ) );

	if( !m_resizedRT )
	{
		m_resizedRT = allocator( ctx )->CreateCustomRenderTarget( outputW, outputH, RenderTarget::RTSemantic::S_DRAW_ONLY );
		m_resizedHorizontallyBluredRT = allocator( ctx )->CreateCustomRenderTarget( outputW, outputH, RenderTarget::RTSemantic::S_DRAW_ONLY );
	}

	enable( ctx, m_resizedHorizontallyBluredRT );
	clearBoundRT( ctx, glm::vec4() );
	disableBoundRT( ctx );

	// Clear blur output render target.
	enable( ctx, m_resizedRT );
	clearBoundRT( ctx, glm::vec4() );

	m_simpleBlitEffect->Render( ctx, input );

	disableBoundRT( ctx );
	return m_resizedRT;
}

// **************************
//
void                    NBlurFSEStep::FreeRenderTargets				( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
Float32					NBlurFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 1 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
bool                    NBlurFSEStep::IsIdle                       ( SceneNodeRepr * ) const
{
	// Nothing to do if blur size is 0.
	return GetBlurSize() == 0;
}

// **************************
// If 
bool                    NBlurFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	// if we've just rendered blured image fo not run next default finalize pass.
	// else run it.
	return GetBlurSize() != 0;
}

} // nrl
} // bv

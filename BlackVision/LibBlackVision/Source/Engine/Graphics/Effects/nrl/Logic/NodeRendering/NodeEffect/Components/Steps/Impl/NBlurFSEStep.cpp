#include "stdafx.h"

#include "NBlurFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include <cmath>


namespace bv { namespace nrl {

// **************************
//
NBlurFSEStep::NBlurFSEStep          ()
    : Parent( nullptr )
	, m_tmpRT( nullptr )
{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blurEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLUR );
	m_simpleBlitEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_SIMPLE_BLIT );
    SetState( m_blurEffect->GetState() );
	GetState()->AppendValue( m_simpleBlitEffect->GetState()->GetValueAt( 0 ) );
	GetState()->AppendValue( m_simpleBlitEffect->GetState()->GetValueAt( 1 ) );
}

// **************************
//
NBlurFSEStep::~NBlurFSEStep           ()
{
    delete m_blurEffect;
	delete m_simpleBlitEffect;
	delete m_tmpRT;
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

		if( !m_tmpRT )
		{
            // FIXME: nrl - MAXDWORD32 - isn't that a hack???
			// m_tmpRT = allocator( ctx )->CreateCustomRenderTarget( MAXDWORD32, RenderTarget::RTSemantic::S_DRAW_ONLY );
			m_tmpRT = allocator( ctx )->CreateCustomRenderTarget( (UInt32)~(UInt32(0)), RenderTarget::RTSemantic::S_DRAW_ONLY );
			enable( ctx, m_tmpRT );
			clearBoundRT( ctx, glm::vec4() );
			disableBoundRT( ctx );
		}

		blurSize /= stepsNum;

		auto tmpOut = m_tmpRT;

		for( auto i = 0; i < stepsNum - 1; ++i )
		{
			enable( ctx, tmpOut );
			clearBoundRT( ctx, glm::vec4() );
			disableBoundRT( ctx );

			FastBlur( ctx, rd, blurSize, tmpOut );
			auto oldInputRT = rd.GetEntry( 0 );
			rd.SetEntry( 0, tmpOut );

			renderer( ctx )->GenerateMipmaps( tmpOut );

			tmpOut = oldInputRT;

			// Clear blur output render target.
			enable( ctx, tmpOut );
			clearBoundRT( ctx, glm::vec4() );
			disableBoundRT( ctx );
		}

		FastBlur( ctx, rd, blurSize, mainRT );
	}

	enable( ctx, mainRT );
}

// **************************
//
void		NBlurFSEStep::FastBlur						( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output )
{
	auto blurSizeW = blurSize;
	auto blurSizeH = blurSize;

	//if( blurSize <= 16 )
	//{
	//	SetQuadTransform( 1.f, 1.f, 0.f, 0.f );
	//	SetUVTransform( 1.f, 1.f, 0.f, 0.f );
	//	return BlurInput( ctx, input, blurSizeW, blurSizeH, output );
	//}
	//else
	{
		NRenderedData inputResized( 1 );

		auto inputW = input.GetEntry( 0 )->Width();
		auto inputH = input.GetEntry( 0 )->Height();

		auto scale = std::max( 1.f, blurSize / 10 );

		auto outputW = UInt32( ceil( inputW / scale ) );
		auto outputH = UInt32( ceil( inputH / scale ) );

		auto scaleW = Float32( outputW ) / Float32( inputW );
		auto scaleH = Float32( outputH ) / Float32( inputH );

		blurSizeW = blurSize * scaleW;
		blurSizeH = blurSize * scaleH;

		RenderTarget * resizedRT = nullptr;

		resizedRT = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

		enable( ctx, resizedRT );
		clearBoundRT( ctx, glm::vec4() );
		disableBoundRT( ctx );

		SetQuadTransform( scaleW, scaleH, 0.f, 0.f );
		SetUVTransform( 1.f, 1.f, 0.f, 0.f );
		ResizeInput( ctx, input, resizedRT );

		NRenderedData inputTmp( 1 );
		inputTmp.SetEntry( 0, resizedRT );

		SetQuadTransform( scaleW, scaleH, 0.f, 0.f );
		SetUVTransform( scaleW, scaleH, 0.f, 0.f );

		// Allocate output render target if not passed
		auto bluredRT = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
		NRenderedData rd( 1 );
		rd.SetEntry( 0, bluredRT );
		enable( ctx, bluredRT );
		clearBoundRT( ctx, glm::vec4() );
		disableBoundRT( ctx );

		BlurInput( ctx, inputTmp, blurSizeW, blurSizeH, bluredRT );

		inputTmp.SetEntry( 0, bluredRT );
		SetQuadTransform( 1.f, 1.f, 0.f, 0.f );
		SetUVTransform( scaleW, scaleH, 0.f, 0.f );
		ResizeInput( ctx, inputTmp, output );

		allocator( ctx )->Free();
		allocator( ctx )->Free();
	}
}

// **************************
//
void					NBlurFSEStep::SetQuadTransform				( Float32 scaleX, Float32 scaleY, Float32 translateX, Float32 translateY )
{
	auto quadTransformVal = GetState()->GetValueAt( 5 );
	QueryTypedValue< ValueVec4Ptr >( quadTransformVal )->SetValue( glm::vec4( scaleX, scaleY, translateX, translateY ) );
	quadTransformVal = GetState()->GetValueAt( 7 );
	QueryTypedValue< ValueVec4Ptr >( quadTransformVal )->SetValue( glm::vec4( scaleX, scaleY, translateX, translateY ) );
}

// **************************
//
void					NBlurFSEStep::SetUVTransform				( Float32 scaleX, Float32 scaleY, Float32 translateX, Float32 translateY )
{
	auto uvTransformVal = GetState()->GetValueAt( 6 );
	QueryTypedValue< ValueVec4Ptr >( uvTransformVal )->SetValue( glm::vec4( scaleX, scaleY, translateX, translateY ) );
	uvTransformVal = GetState()->GetValueAt( 8 );
	QueryTypedValue< ValueVec4Ptr >( uvTransformVal )->SetValue( glm::vec4( scaleX, scaleY, translateX, translateY ) );
}

// **************************
//
void					NBlurFSEStep::BlurInput						( NRenderContext * ctx, const NRenderedData & input, Float32 blurSizeW, Float32 blurSizeH, const RenderTarget * output )
{
	auto wrt = input.GetEntry( 0 )->Width();
	auto hrt = input.GetEntry( 0 )->Height();

	auto blurSizeVal = GetState()->GetValueAt( 1 );
	QueryTypedValue< ValueFloatPtr >( blurSizeVal )->SetValue( blurSizeW );

	auto vertical = GetState()->GetValueAt( 3 );

	// Allocate new render target for vertical blur pass
	auto resizedHorizontallyBluredRT = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

	NRenderedData rd( 1 );
	rd.SetEntry( 0, resizedHorizontallyBluredRT );

	// Clear blur output render target.
	enable( ctx, resizedHorizontallyBluredRT );
	clearBoundRT( ctx, glm::vec4() );

	auto textureSize = GetState()->GetValueAt( 0 );
	QueryTypedValue< ValueVec2Ptr >( textureSize )->SetValue( glm::vec2( wrt, hrt ) );

	// Run vertical blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( true );
	m_blurEffect->Render( ctx, resizedHorizontallyBluredRT, input );
	disableBoundRT( ctx );

	// Run horizontal blur pass
	QueryTypedValue< ValueFloatPtr >( blurSizeVal )->SetValue( blurSizeH );
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( false );
	rd.SetEntry( 0, resizedHorizontallyBluredRT );
	m_blurEffect->Render( ctx, output, rd );
	disableBoundRT( ctx );

	allocator( ctx )->Free();

	//allocator( ctx )->Free();  // free allocated locally render target.
}

// **************************
//
void					NBlurFSEStep::ResizeInput					( NRenderContext * ctx, const NRenderedData & input, const RenderTarget * output )
{
	// Clear blur output render target.
	enable( ctx, output );
	//clearBoundRT( ctx, glm::vec4() );

	m_simpleBlitEffect->Render( ctx, input ); 
	
	disableBoundRT( ctx );
}

// **************************
//
void                    NBlurFSEStep::FreeRenderTargets				( NRenderContext * ctx, const NRenderedData * input )
{
	input;
	ctx;
    //assert( input->GetNumEntries() == GetNumRequiredInputs() );
    //assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    //allocator( ctx )->Free( GetNumRequiredInputs() );
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
// If - FIXME: "If what
bool                    NBlurFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	// if we've just rendered blured image fo not run next default finalize pass.
	// else run it.
	return GetBlurSize() != 0;
}

// **************************
//
void                    NBlurFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_blurEffect->AppendRenderPasses_DIRTY_HACK( passes );
    m_simpleBlitEffect->AppendRenderPasses_DIRTY_HACK( passes );
}

} // nrl
} // bv
